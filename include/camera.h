// Copyright (C) 2023 Aditya Singh

#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "bmp_headers.h"

#include <thread>
#include <vector>
#include <mutex>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include <execution>

/// @brief Represents an image.
class image {
public:
	/// @brief Construct an image with given height and width.
	image(int h, int w) : height(h), width(w)
	{
		pixels = new color[height * width]{{0, 0, 0}};
	}

	/// @brief Destroy the image and free the memory.
	~image() { delete[] pixels; }

	/// @brief Get the color at the given pixel.
	inline color &operator[](int i, int j) const
	{
		return pixels[width * i + j];
	}

	/// @brief Divide colors at all pixels by a scalar.
	inline image &operator/(double scale)
	{
		std::transform(std::execution::par, pixels,
		               pixels + height * width, pixels,
		               [scale](color c) { return c / scale; });
		return *this;
	}

	/// @brief Array of colors representing pixels.
	color *pixels;

	/// @brief Height of the image.
	int height;

	/// @brief Width of the image.
	int width;
};

/**
 * @brief Save an image to a file.
 *
 * @param img The image to save.
 * @param filename The name of the file to save to.
 */
void saveimage(const image &img, const char *filename)
{
	std::ofstream file(filename);
	write_BMP_headers(file, img.width, img.height);
	for (int i = img.height - 1; i >= 0; i--) {
		for (int j = 0; j < img.width; j++)
			write_color(file, img[i, j]);
	}
	file.close();
}

/// @brief Get the number of columns in the terminal.
int tput_cols()
{
	struct winsize w{};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

/// @brief Show rendering progress.
void show_progress(const std::vector<std::atomic<float>> &progs)
{
	unsigned num_threads = progs.size();
	bool done;
	while (true) {
		done = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(100)));
		unsigned cols = tput_cols() - 10;
		if (cols < 15)
			return;
		for (unsigned t_id = 0; t_id < num_threads; t_id++) {
			float p = progs[t_id].load();
			auto num_equals = static_cast<unsigned>(p * cols);
			if (p < 1) {
				done = false;
				printrt("[{}" ESC CSI BACKSPACE ">{}] {:0<6.2f}%\n",
				        std::string(num_equals, '='),
				        std::string(cols - num_equals, ' '),
				        p * 100);
			} else {
				printrt("[{}] {:0<6.2f}%\n",
				        std::string(cols, '='),
				        p * 100);
			}
		}
		if (done)
			return;
		printrt(ESC CSI "{}" PREV_LINE, progs.size());
	}
}

/// @brief Represents a camera.
class camera {
public:
	/**
	 * @brief Construct a camera with the given parameters.
	 *
	 * @param image_width Width of the image in pixels.
	 * @param aspect_ratio Aspect ratio of the image.
	 * @param vfov Field of view.
	 * @param lookfrom Location of the camera.
	 * @param lookat Point the camera is looking at.
	 * @param vup Camera's up vector.
	 * @param num_samples Number of samples per pixel.
	 * @param depth Maximum number of bounces.
	 */
	camera(int    image_width,
	       double aspect_ratio,
	       double vfov,
	       point  lookfrom,
	       point  lookat,
	       vec    vup,
	       int    num_samples = 10,
	       int    depth = 10)
	    : img_width(image_width), fov(vfov), max_depth(depth),
	      num_samples(num_samples)
	{
		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = (img_height < 1) ? 1 : img_height;

		center = lookfrom;
		f = (lookat - lookfrom).length();

		v_width = 2 * f * std::tan(degrees_to_radians(fov) / 2);
		v_height = v_width * static_cast<double>(img_height) / img_width;

		w = unit_vector(lookat - lookfrom);
		u = unit_vector(cross(w, vup));
		v = cross(u, w);

		viewport_u    = v_width * u;
		viewport_v    = v_height * -v;
		pixel_delta_u = viewport_u / img_width;
		pixel_delta_v = viewport_v / img_height;

		viewport_upper_left = center + f * w
		                      - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left
		              + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	/**
	 * @brief Render the scene to a file.
	 *
	 * @param world The hittable list representing the scene.
	 * @param filename The name of the file to save to.
	 * @param num_threads The number of threads to use, defaults to
	 *                    `std::thread::hardware_concurrency() - 2`.
	 */
	void render(const hittable &world, const char *filename,
	            unsigned num_threads = std::thread::hardware_concurrency() - 2);
	void render2(const hittable &world, const char *filename,
	             unsigned num_threads = std::thread::hardware_concurrency() - 2);

private:
	int    num_samples = 1;
	int    img_width;
	int    img_height;
	double f;
	double fov;
	double v_height;
	double v_width;
	point  center;
	vec    viewport_u;
	vec    viewport_v;
	vec    pixel_delta_u;
	vec    pixel_delta_v;
	vec    u;
	vec    v;
	vec    w;
	point  viewport_upper_left;
	point  pixel00_loc;
	int    max_depth;

	/// @brief Get the ray for the given pixel.
	ray   get_ray(int i, int j);

	/// @brief Get the color for the given ray.
	color ray_color(const ray &r, int depth, const hittable &world);
};

void camera::render(const hittable &world, const char *filename,
                    unsigned num_threads)
{
	std::vector<std::thread> threads;
	threads.reserve(num_threads);
	std::vector<std::atomic<float>> progs(num_threads);
	image img = {img_height, img_width};

	pr_info("Render started on {} threads\n", num_threads);
	pr_info("Image size: {} x {}\n", img_width, img_height);
	pr_info("{} samples\n", num_samples);

	auto rt_rows = [this, &world, &img, &progs](int start, int end, int id) {
		for (int i = start; i < end; i++) {
			for (int j = 0; j < img_width; j++) {
				for (int idx = 0; idx < num_samples; idx++) {
					ray r = get_ray(i, j);
					img[i, j] += ray_color(r, max_depth, world);
				}
			}
			progs[id] = static_cast<float>(i - start + 1)
			            / static_cast<float>(end - start);
		}
	};

	const unsigned rows_per_thread = img_height / num_threads;
	for (unsigned t_id = 0; t_id < num_threads; t_id++) {
		unsigned start_row = t_id * rows_per_thread;
		unsigned end_row   = (t_id == num_threads - 1)
		                             ? img_height
		                             : start_row + rows_per_thread;
		threads.emplace_back(rt_rows, start_row, end_row, t_id);
	}

	show_progress(progs);

	for (std::thread &thread : threads)
		thread.join();

	saveimage(img / num_samples, filename);
}

void camera::render2(const hittable &world, const char *filename,
                     unsigned num_threads)
{
	image img = {img_height, img_width};
}

color camera::ray_color(const ray &r, int depth, const hittable &world)
{
	hit_record rec;
	if (depth <= 0)
		return {0, 0, 0};
	// 0.001 and not 0 to solve the shadow acne problem
	if (world.hit(r, interval(0.001, infinity), rec)) {
		ray   scattered;
		color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered))
			return attenuation
			       * ray_color(scattered, depth - 1, world);
		return {0, 0, 0};
	}

	vec    dir = unit_vector(r.direction());
	double a   = 0.5 * (dir.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

ray camera::get_ray(int i, int j)
{
	point pixel_sample =
	        pixel00_loc + (i * pixel_delta_v) + (j * pixel_delta_u);
	double px     = -0.5 + random_double();
	double py     = -0.5 + random_double();
	pixel_sample += px * pixel_delta_v + py * pixel_delta_u;

	return {center, pixel_sample - center};
}

#endif // CAMERA_H

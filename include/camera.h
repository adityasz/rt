/**
 * @file      camera.h
 * @author    Aditya Singh
 * @copyright Copyright (C) 2023 Aditya Singh
 * @date      December 2023
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <vector>
#include <mutex>

#include "rt.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "bmp_header_writer.cpp"

class image {
public:
	image(int h, int w) : height(h), width(w)
	{
		array   = new color[height * width]{{0, 0, 0}};
		mutexes = new std::mutex[height * width];
	}

	~image() { delete[] array; }

	inline color &operator[](int i, int j) const
	{
		return array[width * i + j];
	}

	std::mutex& lock(int i, int j) const
	{
		return mutexes[width * i + j];
	}

	color *array;
	std::mutex *mutexes;
	int height;
	int width;
};

class camera {
public:
	camera(int i_width, double aspect_ratio, double v_width,
	       double f, point center, int num = 1, int max_d = 10)
		: img_width(i_width), viewport_width(v_width), max_depth(max_d),
		  focal_length(f), center(center), num_samples(num)
	{
		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = (img_height < 1) ? 1 : img_height;

		viewport_height = viewport_width *
		                  static_cast<double>(img_height) / img_width;

		viewport_u    = vec(viewport_width, 0, 0);
		viewport_v    = vec(0, -viewport_height, 0);
		pixel_delta_u = viewport_u / img_width;
		pixel_delta_v = viewport_v / img_height;

		viewport_upper_left = center + vec(0, 0, -focal_length)
		                      - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left
		              + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	void render(const hittable &world, const char *filename);
	void render_multi_threaded(const hittable &world, const char *filename);
	// FIXME: SIGABRTed when joining threads
	void render_multi_threaded2(const hittable &world, const char *filename);

private:

	int    num_samples = 1;
	int    img_width;
	int    img_height;
	double focal_length;
	double viewport_height;
	double viewport_width;
	point  center;
	vec    viewport_u;
	vec    viewport_v;
	vec    pixel_delta_u;
	vec    pixel_delta_v;
	point  viewport_upper_left;
	point  pixel00_loc;
	int    max_depth;

	ray   get_ray(int i, int j);
	color ray_color(const ray &r, int depth, const hittable &world);
};

void camera::render_multi_threaded(const hittable &world, const char *filename)
{
	const int num_threads = 20;
	const int segment_height = img_height / num_threads;
	std::vector<std::thread> threads;
	image img = {img_height, img_width};

	STATUS_MSG("Render started on " << num_threads << " threads\n");
	STATUS_MSG("Image size: " << img_width << " x " << img_height << '\n');
	STATUS_MSG(num_samples << " samples\n");

	auto raytrace_segment = [this, &world, &img](int start_row,
	                                             int end_row,
	                                             int id) {
		for (int i = start_row; i < end_row; i++) {
			for (int j = 0; j < img_width; j++) {
				for (int idx = 0; idx < num_samples; idx++) {
					ray r = get_ray(i, j);
					img[i, j] += ray_color(r, max_depth, world);
				}
			}
		}
	};

	for (int thread_id = 0; thread_id < num_threads; thread_id++) {
		int start_row = thread_id * segment_height;
		int end_row   = (thread_id == num_threads - 1)
		                ? img_height
		                : start_row + segment_height;
		threads.emplace_back(raytrace_segment, start_row,
		                     end_row, thread_id);
	}

	for (std::thread &thread : threads)
		thread.join();

	std::ofstream file(filename);
	write_BMP_headers(file, img_width, img_height);
	for (int i = img_height - 1; i >= 0; i--) {
		for (int j = 0; j < img_width; j++)
			write_color(file, img[i, j], num_samples);
	}
	file.close();
}

// FIXME: Bad renders; something is broken (probably locks: read up on threads)
void camera::render_multi_threaded2(const hittable &world, const char *filename)
{
	const int num_threads = 20;
	const int samples_per_thread = num_samples / num_threads;

	std::vector<std::thread> threads;
	image img = {img_height, img_width};

	STATUS_MSG("Render started on " << num_threads << " threads\n");
	STATUS_MSG("Image size: " << img_width << " x " << img_height << '\n');
	STATUS_MSG(num_samples << " samples\n");

	auto raytrace = [&]() {
		for (int i = 0; i < img_height; i++) {
			for (int j = 0; j < img_width; j++) {
				for (int idx = 0; idx < samples_per_thread; idx++) {
					ray r = get_ray(i, j);
					// std::lock_guard<std::mutex> guard(img.lock(i, j));
					img[i, j] += ray_color(r, max_depth, world);
				}
			}
		}
	};
	for (int thread_id = 0; thread_id < num_threads; thread_id++)
		threads.emplace_back(raytrace);
	for (std::thread &thread : threads)
		thread.join();

	std::ofstream file(filename);
	write_BMP_headers(file, img_width, img_height);
	for (int i = img_height - 1; i >= 0; i--) {
		for (int j = 0; j < img_width; j++)
			write_color(file, img[i, j], num_samples);
	}
	file.close();
}

void camera::render(const hittable &world, const char *filename)
{
	std::ofstream file(filename);
	write_BMP_headers(file, img_width, img_height);
	for (int i = img_height - 1; i >= 0; i--) {
		STATUS_MSG("\rScanlines completed: " << img_height - i
		        << '/' << img_height << std::flush);
		for (int j = 0; j < img_width; j++) {
			color pixel_color(0, 0, 0);
			for (int idx = 0; idx < num_samples; idx++) {
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world);
			}
			write_color(file, pixel_color, num_samples);
		}

		const uint8_t padding = 0;
		for (int p = 0; p < (4 - (3 * img_width) % 4) % 4; p++) {
			file.write(std::bit_cast<const char *>(&padding),
			           sizeof(padding));
		}
	}
	file.close();
	STATUS_MSG("\rDone                                      " << std::endl);
}

color camera::ray_color(const ray &r, int depth, const hittable &world)
{
	hit_record rec;
	if (depth <= 0)
		return {0, 0, 0};
	// 0.001 and not 0 to solve the shadow acne problem
	if (world.hit(r, interval(0.001, infinity), rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, depth - 1,
						       world);
		return {0, 0, 0};
	}

	vec    dir = unit_vector(r.direction());
	double a   = 0.5 * (dir.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

ray camera::get_ray(int i, int j)
{
	point pixel_sample = pixel00_loc + (i * pixel_delta_v)
	                     + (j * pixel_delta_u);
	double px = -0.5 + random_double();
	double py = -0.5 + random_double();
	pixel_sample += px * pixel_delta_v + py * pixel_delta_u;

	return {center, pixel_sample - center};
}

#endif //CAMERA_H

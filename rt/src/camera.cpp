#include "bmp_headers.h"
#include "camera.h"
#include "print.h"

#include <thread>
#include <opencv2/opencv.hpp>

#define ESC "\033"
#define CSI "["
#define PREV_LINE "F"
#define BACKSPACE "D"

using namespace rt;

/**
 * @brief Save an image to a file.
 *
 * @param img The image to save.
 * @param filename The name of the file to save to.
 */
static void saveimage(const image &img, const std::string &filename)
{
	std::ofstream file(filename);
	if (std::string format = filename.substr(filename.rfind('.')); format == ".png") {
		std::transform(std::execution::par, img.pixels,
			           img.pixels + img.height * img.width, img.pixels,
			           [](const color &c) { return linear_to_gamma(c); });
		cv::Mat output(img.height, img.width, CV_8UC3);
		uchar* out_ptr = output.data;
		for (int i = 0; i < img.height; i++) {
			for (int j = 0; j < img.width; j++) {
				int idx = 3 * (i * img.width + j);
				out_ptr[idx + 0] = static_cast<uchar>(255 * img[i, j][2]);
				out_ptr[idx + 1] = static_cast<uchar>(255 * img[i, j][1]);
				out_ptr[idx + 2] = static_cast<uchar>(255 * img[i, j][0]);
			}
		}
		cv::imwrite(filename, output);
	} else if (format == ".bmp") {
		write_BMP_headers(file, img.width, img.height);
		for (int i = img.height - 1; i >= 0; i--) {
			for (int j = 0; j < img.width; j++)
				write_color_bmp(file, img[i, j]);
		}
	}
	file.close();
}

/// @brief Get the number of columns in the terminal.
static int tput_cols()
{
	winsize w{};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

/// @brief Show rendering progress.
static void show_progress(const std::vector<std::atomic<float>> &progs)
{
	if constexpr(LOGLEVEL < LogLevel::Info)
		return;

	unsigned num_threads = progs.size();
	while (true) {
		bool done = true;
		std::this_thread::sleep_for(
		    std::chrono::milliseconds(static_cast<long long>(100)));
		unsigned cols = tput_cols() - 10;
		if (cols < 15)
			return;
		for (unsigned t_id = 0; t_id < num_threads; t_id++) {
			float p = progs[t_id].load();
			float progress = p * 100;
			auto num_equals = static_cast<unsigned>(p * static_cast<float>(cols));
			if (p < 1) {
				done = false;
				unsigned num_spaces = cols - num_equals;
				rt::print<Info>("[{:=^{}}" ESC CSI BACKSPACE ">{: ^{}}] {:0<6.2f}%\n",
				                "", num_equals, "", num_spaces, progress);
			} else {
				rt::print<Info>("[{:=^{}}] {:0<6.2f}%\n", "", cols, progress);
			}
		}
		if (done)
			return;
		rt::print<Info>(ESC CSI "{}" PREV_LINE, progs.size());
	}
}

void camera::render(const hittable &world, const std::string &filename, unsigned num_threads)
{
	std::vector<std::thread> threads;
	threads.reserve(num_threads);
	std::vector<std::atomic<float>> progs(num_threads);
	image img = {img_height, img_width};

	rt::print<Info>("Render started on {} threads\n", num_threads);
	rt::print<Info>("Image size: {} x {}\n", img_width, img_height);
	rt::print<Info>("{} samples\n", num_samples);

	auto rt_rows = [this, &world, &img, &progs](int start, int end, int id) {
		for (int i = start; i < end; i++) {
			for (int j = 0; j < img_width; j++) {
				for (int idx = 0; idx < num_samples; idx++) {
					ray r = get_ray(i, j);
					img[i, j] += ray_color(r, max_depth, world);
				}
			}

			if constexpr(LOGLEVEL >= LogLevel::Info) {
				progs[id] = static_cast<float>(i - start + 1)
							/ static_cast<float>(end - start);
			}
		}
	};

	const unsigned rows_per_thread = img_height / num_threads;
	for (unsigned t_id = 0; t_id < num_threads; t_id++) {
		unsigned start_row = t_id * rows_per_thread;
		unsigned end_row   = t_id == num_threads - 1 ? img_height
		                                             : start_row + rows_per_thread;
		threads.emplace_back(rt_rows, start_row, end_row, t_id);
	}

	show_progress(progs);

	for (std::thread &thread : threads)
		thread.join();

	saveimage(img / num_samples, filename);
}

color camera::ray_color(const ray &r, int depth, const hittable &world)
{
	if (depth <= 0)
		return {0, 0, 0};
	// 0.001 and not 0 to solve the shadow acne problem
	if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
		color attenuation;
		if (ray scattered; rec.mat->scatter(r, rec, attenuation, scattered))
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
	point pixel_sample = pixel00_loc + i * pixel_delta_v
	                     + j * pixel_delta_u;
	double px     = -0.5 + random_double();
	double py     = -0.5 + random_double();
	pixel_sample += px * pixel_delta_v + py * pixel_delta_u;

	return {center, pixel_sample - center};
}

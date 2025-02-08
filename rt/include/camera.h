#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "material.h"

#include <thread>
#include <sys/ioctl.h>
#include <algorithm>
#include <execution>

namespace rt
{
inline double deg_to_rad(double degrees)
{
	constexpr double pi = 3.141'592'653'589'793'238'462'643'383;
	return degrees * pi / 180.0;
}

/// @brief Represents an image.
class image {
public:
	color *pixels; //< @brief The array of colors representing pixels.
	int    height; //< @brief The height of the image.
	int    width;  //< @brief The width of the image.

	/// @brief Construct an image with given height and width.
	image(int h, int w) : height(h), width(w)
	{
		pixels = new color[height * width]{{0, 0, 0}};
	}

	/// @brief Destroy the image and free the memory.
	~image() { delete[] pixels; }

	/// @brief Get the color at the given pixel.
	color &operator[](int i, int j) const
	{
		return pixels[width * i + j];
	}

	/// @brief Divide colors at all pixels by a scalar.
	image &operator/(double scale)
	{
		std::transform(std::execution::par, pixels,
			           pixels + height * width, pixels,
			           [scale](const color &c) { return c / scale; });
		return *this;
	}
};

/// @brief Represents a camera.
class camera {
	int    num_samples;
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
	static color ray_color(const ray &r, int depth, const hittable &world);

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
	camera(int          image_width,
	       double       aspect_ratio,
	       double       vfov,
	       const point &lookfrom,
	       const point &lookat,
	       const vec   &vup,
	       int          num_samples = 10,
	       int          depth = 10)
		: num_samples(num_samples), img_width(image_width), fov(vfov), max_depth(depth)
	{
		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = (img_height < 1) ? 1 : img_height;

		center = lookfrom;
		f = (lookat - lookfrom).length();

		v_width = 2 * f * std::tan(deg_to_rad(fov) / 2);
		v_height = v_width * static_cast<double>(img_height) / img_width;

		w = unit_vector(lookat - lookfrom);
		u = unit_vector(cross(w, vup));
		v = cross(u, w);

		viewport_u = v_width * u;
		viewport_v = v_height * -v;
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
	 * @param num_threads The number of threads to use.
	 */
	void render(const hittable &world, const std::string &filename,
	            unsigned num_threads = std::thread::hardware_concurrency());
};
}

#endif // CAMERA_H

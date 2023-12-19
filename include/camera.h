/**
 * @file      camera.h
 * @author    Aditya Singh
 * @copyright Copyright (C) 2023 Aditya Singh
 * @date      December 2023
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "bmp_header_writer.cpp"

class camera {
public:
	camera(int i_width, double aspect_ratio, double v_width,
	       double f, point center)
		: img_width(i_width), viewport_width(v_width),
		  focal_length(f), center(center)
	{
		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = (img_height < 1) ? 1 : img_height;

		viewport_height = viewport_width *
		                  static_cast<double>(img_height) / img_width;

		viewport_u    = vec(viewport_width, 0, 0);
		viewport_v    = vec(0, -viewport_height, 0);
		pixel_delta_u = viewport_u / img_width;
		pixel_delta_v = viewport_v / img_height;

		viewport_upper_left = center - vec(0, 0, focal_length)
		                      - viewport_u / 2 - viewport_v / 2;
		pixel00_loc         = viewport_upper_left
		                      + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	void render(const hittable &world, std::ofstream &file);

private:
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

	static color ray_color(const ray &r, const hittable &world);
};

void camera::render(const hittable &world, std::ofstream &file)
{
	write_BMP_headers(file, img_width, img_height);
	for (int i = img_height - 1; i >= 0; i--) {
		STATUS_MSG("\rScanlines completed: " << img_height - i
		        << '/' << img_height << std::flush);
		for (int j = 0; j < img_width; j++) {
			point pixel_center  = pixel00_loc + (j * pixel_delta_u)
			                      + (i * pixel_delta_v);
			vec   ray_direction = pixel_center - center;
			ray   r(center, ray_direction);

			color pixel_color = ray_color(r, world);
			write_color(file, pixel_color);
		}

		const uint8_t padding = 0;
		for (int p = 0; p < (4 - (3 * img_width) % 4) % 4; p++) {
			file.write(std::bit_cast<const char *>(&padding),
			           sizeof(padding));
		}
	}
	std::clog << std::endl;
}

color camera::ray_color(const ray &r, const hittable &world)
{
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec))
		return 0.5 * (rec.normal + color(1, 1, 1));

	vec    dir = unit_vector(r.direction());
	double a   = 0.5 * (dir.y() + 1.0);

	return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

#endif //CAMERA_H

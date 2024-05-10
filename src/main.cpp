// Copyright (C) 2023 Aditya Singh

#include "rt.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

int main()
{
	double aspect_ratio   = 16.0 / 9.0;
	int    img_width      = 2560;
	double viewport_width = 3.0; // 0.002
	double focal_length   = 1.0; // 0.001
	int    num_samples    = 40;
	point  camera_center(0, 0, 0);

	camera cam(img_width, aspect_ratio, viewport_width,
	           focal_length, camera_center, num_samples);

	hittable_list world;

	auto mat_ground = std::make_shared<lambertian>(rgb(90, 55, 36));
	auto mat_center = std::make_shared<lambertian>(rgb(226, 140, 141));
	auto mat_left   = std::make_shared<dielectric>(1.5);
	auto mat_right  = std::make_shared<metal>(rgb(28, 91, 155), 0.01);

	world.add(
	        std::make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, mat_ground),
		std::make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, mat_center),
		std::make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, mat_left),
		std::make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, mat_right)
	);

	auto start = std::chrono::steady_clock::now();
	cam.render(world, "image.bmp");
	auto end      = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<double>(end - start);
	fmt::print("Rendered in {:.2f} s\n", duration.count());
}

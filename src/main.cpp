#include <iostream>

#include "rt.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	double aspect_ratio   = 16.0 / 9.0;
	int    img_width      = 2560;
	double viewport_width = 3.0;          // 0.002
	double focal_length   = 1.0;          // 0.001
	int    num_samples    = 40;
	point  camera_center(0, 0, 0);

	camera cam(img_width, aspect_ratio, viewport_width,
	           focal_length, camera_center, num_samples);

	hittable_list world;

	auto mat_ground = std::make_shared<lambertian>(rgb(90, 55, 36));
	auto mat_center = std::make_shared<lambertian>(rgb(226, 140, 141));
	// auto mat_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.01);
	auto mat_left   = std::make_shared<dielectric>(1.5);
	auto mat_right  = std::make_shared<metal>(rgb(28, 91, 155), 0.01);

	world.add(std::make_shared<sphere>(point( 0.0, -100.25, -1.0), 100.00, mat_ground));
	world.add(std::make_shared<sphere>(point(-0.5,    0.00, -1.0),   0.25, mat_left));
	world.add(std::make_shared<sphere>(point( 0.0,    0.00, -1.0),   0.25, mat_center));
	world.add(std::make_shared<sphere>(point( 0.5,    0.00, -1.0),   0.25, mat_right));

#ifdef DEBUG
	auto start    = std::chrono::steady_clock::now();
	cam.render_multi_threaded(world, "image.bmp");
	auto end      = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<double>(end - start);
	DEBUG_MSG("Render time: " << duration.count() << " s\n");
#else
	cam.render_multi_threaded(world, "image.bmp");
#endif
}

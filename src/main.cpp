#include "rt.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	double aspect_ratio   = 16.0 / 9.0;
	int    img_width      = 2'560;
	double viewport_width = 2.0;
	double focal_length   = 1.0;
	int    num_samples    = 100;
	int    depth          = 50;
	point  camera_center(0, 0, 2);

	camera cam(img_width, aspect_ratio, viewport_width, focal_length,
	           camera_center, num_samples, depth);

	hittable_list world;

	auto mat_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto mat_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto mat_left   = std::make_shared<dielectric>(1.5);
	auto mat_bubble = std::make_shared<dielectric>(1. / 1.5);
	auto mat_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(std::make_shared<sphere>(point(0, -100.5, -1), 100.0, mat_ground),
	          std::make_shared<sphere>(point(0, 0.0, -1), 0.5, mat_center),
	          std::make_shared<sphere>(point(-1, 0.0, -1), 0.5, mat_left),
	          std::make_shared<sphere>(point(-1, 0.0, -1), 0.4, mat_bubble),
	          std::make_shared<sphere>(point(1, 0.0, -1), 0.5, mat_right));

	auto start = std::chrono::steady_clock::now();
	cam.render(world, "image.bmp");
	auto end      = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<double>(end - start);
	fmt::print("Rendered in {:.2f} s\n", duration.count());
}

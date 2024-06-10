#include "rt.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	int    img_width    = 1280;
	double aspect_ratio = 16. / 9.;
	double vfov         = 20;
	auto   lookfrom     = 2 * point(13, 2, 3);
	auto   lookat       = 2 * point(0, 0, 0);
	vec    vup(0, 1, 0);
	int    num_samples = 10;
	int    depth       = 50;


	camera cam(img_width, aspect_ratio, vfov, lookfrom, lookat, vup,
	           num_samples, depth);

	hittable_list world;

	auto ground_material =
	        std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<sphere>(point(0, -1000, 0), 1000,
	                                   ground_material));

	for (int a = -6; a < 6; a++) {
		for (int b = -6; b < 6; b++) {
			auto  choose_mat = random_double();
			point center(a + 0.9 * random_double(), 0.2,
			             b + 0.9 * random_double());

			if ((center - point(4, 0.2, 0)).length() <= 0.9)
				continue;
			std::shared_ptr<material> sphere_material;

			if (choose_mat < 0.8) {
				// diffuse
				auto albedo = color::random() * color::random();
				sphere_material =
					std::make_shared<lambertian>(albedo);
				world.add(std::make_shared<sphere>(
				        center, 0.2, sphere_material));
			} else if (choose_mat < 0.95) {
				// metal
				auto albedo = color::random(0.5, 1);
				auto fuzz   = random_double(0, 0.5);
				sphere_material =
					std::make_shared<metal>(albedo, fuzz);
				world.add(std::make_shared<sphere>(
					center, 0.2, sphere_material));
			} else {
				// glass
				sphere_material =
					std::make_shared<dielectric>(1.5);
				world.add(std::make_shared<sphere>(
					center, 0.2, sphere_material));
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(point(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(point(4, 1, 0), 1.0, material3));

	auto start = std::chrono::steady_clock::now();
	cam.render(world, "image.bmp", std::thread::hardware_concurrency());
	auto end      = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<double>(end - start);
	fmt::print("Rendered in {:.2f} s\n", duration.count());
}

#include <argparse/argparse.hpp>
#include <print>
#include <rt.h>


static void parse_args(int argc, char *argv[], argparse::ArgumentParser &program)
{
	program.add_argument("--width").default_value(2560);
	program.add_argument("--height").default_value(1440);
	program.add_argument("-n", "--num-samples").default_value(40);
	program.add_argument("-d", "--depth").default_value(50);
	program.add_argument("output").required();

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error &err) {
		std::println(stderr, "{}", err.what());
		std::cerr << program;
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	argparse::ArgumentParser program{};
	parse_args(argc, argv, program);

	int     img_width    = program.get<int>("width");
	double  aspect_ratio = static_cast<double>(img_width) / program.get<int>("height");
	double  vfov         = 20;
	auto    lookfrom     = 2 * rt::point(13, 2, 3);
	auto    lookat       = 2 * rt::point(0, 0, 0);
	int     num_samples  = program.get<int>("num-samples");
	int     depth        = program.get<int>("depth");
	rt::vec vup(0, 1, 0);


	rt::camera cam(img_width, aspect_ratio, vfov, lookfrom,
	               lookat, vup, num_samples, depth);

	rt::hittable_list world;

	auto ground_material =
	        std::make_shared<rt::lambertian>(rt::color(0.5, 0.5, 0.5));
	world.add(std::make_shared<rt::sphere>(rt::point(0, -1000, 0), 1000,
	                                       ground_material));

	for (int a = -6; a < 6; a++) {
		for (int b = -6; b < 6; b++) {
			auto choose_mat = rt::random_double();
			rt::point center(a + 0.9 * rt::random_double(), 0.2,
			                 b + 0.9 * rt::random_double());

			if ((center - rt::point(4, 0.2, 0)).length() <= 0.9)
				continue;
			std::shared_ptr<rt::material> sphere_material;

			if (choose_mat < 0.8) {
				// diffuse
				auto albedo = rt::color::random() * rt::color::random();
				sphere_material =
					std::make_shared<rt::lambertian>(albedo);
				world.add(std::make_shared<rt::sphere>(
				        center, 0.2, sphere_material));
			} else if (choose_mat < 0.95) {
				// metal
				auto albedo = rt::color::random(0.5, 1);
				auto fuzz   = rt::random_double(0, 0.5);
				sphere_material =
					std::make_shared<rt::metal>(albedo, fuzz);
				world.add(std::make_shared<rt::sphere>(
					center, 0.2, sphere_material));
			} else {
				// glass
				sphere_material =
					std::make_shared<rt::dielectric>(1.5);
				world.add(std::make_shared<rt::sphere>(
					center, 0.2, sphere_material));
			}
		}
	}

	auto material1 = std::make_shared<rt::dielectric>(1.5);
	world.add(std::make_shared<rt::sphere>(rt::point(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<rt::lambertian>(rt::color(0.4, 0.2, 0.1));
	world.add(std::make_shared<rt::sphere>(rt::point(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<rt::metal>(rt::color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<rt::sphere>(rt::point(4, 1, 0), 1.0, material3));

	auto start    = std::chrono::steady_clock::now();
	cam.render(world, program.get<std::string>("output"));
	auto end      = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<double>(end - start);
	std::print("Rendered in {:.2f} s\n", duration.count());
}

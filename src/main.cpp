#include <iostream>
#include <fstream>

#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray &r, const hittable &world)
{
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec))
		return 0.5 * (rec.normal + color(1, 1, 1));

	vec    dir = unit_vector(r.direction());
	double a   = 0.5 * (dir.y() + 1.0);

	return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main()
{
	double aspect_ratio   = 4.0 / 3.0;
	int    img_width      = 4000;
	double viewport_width = 2.0;
	double focal_length   = 1.0;
	point  camera_center(0, 0, 0);

	camera cam(img_width, aspect_ratio, viewport_width,
		   focal_length, camera_center);

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(point(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point(0, -100.5, -1), 100));

	std::ofstream out_file("image.bmp");
	cam.render(world, out_file);
}

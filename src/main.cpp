#include <iostream>
#include "vec.h"
#include "color.h"
#include "ray.h"

#define DEBUG_STATUS    1
#define DEBUG_FULL      2

#if DEBUG >= DEBUG_STATUS
#define STATUS_MSG(x)                                       \
        do {                                                \
                std::clog << x;                             \
        } while (0)
#define DEBUG_MSG(x)                                        \
        do {                                                \
                if constexpr (DEBUG >= DEBUG_FULL) {        \
                        std::cerr << x;                     \
                }                                           \
        } while (0)
#else
#define STATUS_MSG(x)	do {} while(0)
#define DEBUG_MSG(x)	do {} while(0)
#endif

double hits_sphere(const point &center, double radius, const ray &r)
{
	vec oc = r.origin() - center;

	double a = dot(r.direction(), r.direction());
	double b = dot(r.direction(), oc);
	double c = dot(oc, oc) - radius*radius;
	double discriminant = b*b - a*c;

	if (discriminant < 0)
		return -1.0;

	return (-b - discriminant) / a;
}

color ray_color(const ray &r)
{
	double t = hits_sphere(point(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vec N = unit_vector(r.at(t) - vec(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec    dir = unit_vector(r.direction());
	double a   = 0.5 * (dir.y() + 1.0);
	return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main()
{
	double aspect_ratio = 16.0 / 9.0;
	int    img_width    = 1024;

	// Calculate image height
	int img_height = static_cast<int>(img_width / aspect_ratio);
	img_height = (img_height < 1) ? 1 : img_height;

	// Camera
	double focal_length    = 1.0;
	double viewport_height = 2.0;
	double viewport_width  = viewport_height *
	                          (static_cast<double>(img_width) / img_height);
	point  camera_center   = point(0, 0, 0);

	// Viewport vectors
	vec viewport_u    = vec(viewport_width, 0, 0);
	vec viewport_v    = vec(0, -viewport_height, 0);
	vec pixel_delta_u = viewport_u / img_width;
	vec pixel_delta_v = viewport_v / img_height;

	// Upper left pixel
	point viewport_upper_left = camera_center - vec(0, 0, focal_length)
	                            - viewport_u / 2 - viewport_v / 2;
	point pixel00_loc = viewport_upper_left
	                    + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::ofstream out_file("image.ppm");
	out_file << "P3\n" << img_width << ' ' << img_height << "\n255\n";

	for (int i = 0; i < img_height; i++) {
		STATUS_MSG("\rScanlines completed: " << i + 1 << "/"
		        << img_height << std::flush);
		for (int j = 0; j < img_width; j++) {
			point pixel_center  = pixel00_loc + (j * pixel_delta_u)
			                     + (i * pixel_delta_v);
			vec ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(out_file, pixel_color);
		}
	}
	std::clog << std::endl;
}

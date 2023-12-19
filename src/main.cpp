#include <iostream>
#include <fstream>

#include "rtweekend.h"
#include "bmp_header_writer.cpp"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

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
	double aspect_ratio = 4.0 / 3.0;
	int    img_width    = 4000;

	// Calculate image height
	int img_height = static_cast<int>(img_width / aspect_ratio);
	img_height = (img_height < 1) ? 1 : img_height;

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(point(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point(0, -100.5, -1), 100));

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

	// This is an example of a really really long line, which is intentionally long, so that it crosses 100 columns!

	// Upper left pixel
	point viewport_upper_left = camera_center - vec(0, 0, focal_length)
	                            - viewport_u / 2 - viewport_v / 2;
	point pixel00_loc = viewport_upper_left
	                    + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::ofstream out_file("image.bmp");
	write_BMP_headers(out_file, img_width, img_height);
	for (int i = img_height - 1; i >= 0; i--) {
		STATUS_MSG("\rScanlines completed: " << img_height - i << '/'
		        << img_height << std::flush);
		for (int j = 0; j < img_width; j++) {
			point pixel_center  = pixel00_loc + (j * pixel_delta_u)
					       + (i * pixel_delta_v);
			vec   ray_direction = pixel_center - camera_center;
			ray   r(camera_center, ray_direction);

			color pixel_color = ray_color(r, world);
			write_color(out_file, pixel_color);
		}

		const uint8_t padding = 0;
		for (int p = 0; p < (4 - (3 * img_width) % 4) % 4; p++) {
			out_file.write(std::bit_cast<const char *>(&padding),
			               sizeof(padding));
		}
	}
	std::clog << std::endl;
}

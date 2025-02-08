#include "color.h"
#include "interval.h"

#include <fstream>

using namespace rt;

color rt::linear_to_gamma(const color &pixel_color)
{
	return {std::sqrt(pixel_color.x()),
	        std::sqrt(pixel_color.y()),
	        std::sqrt(pixel_color.z())};
}

void rt::write_color_bmp(std::ofstream &file, color pixel_color)
{
	pixel_color = linear_to_gamma(pixel_color);

	const interval intensity(0.000, 0.999);
	auto b = static_cast<uint8_t>(256 * intensity.clamp(pixel_color.z()));
	auto g = static_cast<uint8_t>(256 * intensity.clamp(pixel_color.y()));
	auto r = static_cast<uint8_t>(256 * intensity.clamp(pixel_color.x()));

	file.write(std::bit_cast<const char *>(&b), sizeof(b));
	file.write(std::bit_cast<const char *>(&g), sizeof(g));
	file.write(std::bit_cast<const char *>(&r), sizeof(r));
}

void rt::write_color_ppm(std::ofstream &file, color pixel_color)
{
	pixel_color = linear_to_gamma(pixel_color);

	const interval intensity(0.000, 0.999);
	auto r = static_cast<int>(256 * intensity.clamp(pixel_color.x()));
	auto g = static_cast<int>(256 * intensity.clamp(pixel_color.y()));
	auto b = static_cast<int>(256 * intensity.clamp(pixel_color.z()));

	file << r << ' ' << g << ' ' << b << '\n';
}

color rt::rgb(int r, int g, int b)
{
	return {(r / 255.0), (g / 255.0), (b / 255.0)};
}

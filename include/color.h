// Copyright (C) 2023 Aditya Singh

#ifndef COLOR_H
#define COLOR_H

#include "vec.h"

#include <iostream>
#include <fstream>
#include <cstdint>

/**
 * @brief Represents colors in (R, G, B).
 *
 * The `color` alias is a convenient shorthand for representing colors in
 * (R, G, B). It uses the same underlying data and operations as the `vec`
 * class.
 */
using color = vec;

inline color linear_to_gamma(color pixel_color)
{
	return {std::sqrt(pixel_color.x()),
	        std::sqrt(pixel_color.y()),
	        std::sqrt(pixel_color.z())};
}

/**
 * @brief Print colors to an output file in bottom-up order (for BMP).
 *
 * @param out The output file.
 * @param pixel_color The color.
 */
void write_color(std::ofstream &file, color pixel_color)
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

/**
 * @brief Print colors to an output file (PPM).
 *
 * @param out The output file.
 * @param pixel_color The color.
 */
void write_color_ppm(std::ofstream &file, color pixel_color)
{
	pixel_color = linear_to_gamma(pixel_color);

	const interval intensity(0.000, 0.999);
	auto r = static_cast<int>(256 * intensity.clamp(pixel_color.x()));
	auto g = static_cast<int>(256 * intensity.clamp(pixel_color.y()));
	auto b = static_cast<int>(256 * intensity.clamp(pixel_color.z()));

	file << r << ' ' << g << ' ' << b << '\n';
}

inline color rgb(int r, int g, int b)
{
	return {static_cast<double>(r / 255.0),
		static_cast<double>(g / 255.0),
		static_cast<double>(b / 255.0)};
}

#endif // COLOR_H

/**
 * @file 	color.h
 * @author 	Aditya Singh
 * @copyright	Copyright (C) 2023 Aditya Singh
 * @date 	December 2023
 */

#ifndef COLOR_H
#define COLOR_H

#include "vec.h"
#include <iostream>
#include <fstream>

/**
 * @brief Represents colors in (R, G, B).
 *
 * The `color` alias is a convenient shorthand for representing colors in
 * (R, G, B). It uses the same underlying data and operations as the `vec`
 * class.
 */
using color = vec;

/**
 * @brief Print colors to an output stream (for use with PPM et al.).
 *
 * @param out The output stream (e.g., std::cout).
 * @param pixel_color The color.
 */
void write_color(std::ostream &out, color pixel_color)
{
	out << static_cast<int>(255.999 * pixel_color.x()) << ' '
	    << static_cast<int>(255.999 * pixel_color.y()) << ' '
	    << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

/**
 * @brief Print colors to an output file in bottom-up order (for BMP).
 *
 * @param out The output file.
 * @param pixel_color The color.
 */
void write_color(std::ofstream &file, color pixel_color)
{
	auto blue  = static_cast<uint8_t>(255.999 * pixel_color.z());
	auto green = static_cast<uint8_t>(255.999 * pixel_color.y());
	auto red   = static_cast<uint8_t>(255.999 * pixel_color.x());

	file.write(std::bit_cast<const char *>(&blue),  sizeof(blue));
	file.write(std::bit_cast<const char *>(&green), sizeof(green));
	file.write(std::bit_cast<const char *>(&red),   sizeof(red));
}

#endif

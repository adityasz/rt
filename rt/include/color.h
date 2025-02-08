#ifndef COLOR_H
#define COLOR_H

#include "vec.h"

namespace rt
{
/**
 * @brief Represents colors in (R, G, B).
 *
 * The `color` alias is a convenient shorthand for representing colors in
 * (R, G, B). It uses the same underlying data and operations as the `vec`
 * class.
 */
using color = vec;

color linear_to_gamma(const color &pixel_color);

/**
 * @brief Print colors to an output file in bottom-up order (for BMP).
 *
 * @param file The output file.
 * @param pixel_color The color.
 */
void write_color_bmp(std::ofstream &file, color pixel_color);


/**
 * @brief Print colors to an output file (PPM).
 *
 * @param file The output file.
 * @param pixel_color The color.
 */
void write_color_ppm(std::ofstream &file, color pixel_color);

color rgb(int r, int g, int b);
}

#endif // COLOR_H

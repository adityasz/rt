#ifndef BMP_HEADERS_H
#define BMP_HEADERS_H

#include <fstream>

namespace rt
{
/**
 * @brief Writes the headers of a BMP file.
 *
 * This function writes the headers of a BMP file to an output stream.
 * The headers include both the file header and the DIB header.
 *
 * @param image The output stream to write the headers to.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 */
void write_BMP_headers(std::ofstream &image, int width, int height);
}

#endif // BMP_HEADERS_H

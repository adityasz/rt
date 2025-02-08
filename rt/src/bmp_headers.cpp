#include "bmp_headers.h"

void rt::write_BMP_headers(std::ofstream &image, int width, int height)
{
	struct Header {
		Header(uint32_t width, uint32_t height)
		    : file_size(54 + 3 * width * height) {}

		const uint16_t file_type = 0x4D42;
		const uint32_t file_size;
		const uint16_t reserved1 = 0;
		const uint16_t reserved2 = 0;
		const uint32_t px_offset = 54;
	} __attribute__((packed)) header(width, height);
	image.write(std::bit_cast<const char *>(&header), sizeof(header));

	struct DIB_Header {
		DIB_Header(uint32_t width, uint32_t height)
		    : img_width(width), img_height(height) {}

		const uint32_t header_size        = 40;
		const uint32_t img_width;
		const uint32_t img_height;
		const uint16_t num_color_planes   = 1;
		const uint16_t bit_depth          = 24;
		const uint32_t compression_method = 0;
		const uint32_t image_size         = 0;
		const uint32_t px_per_m_x         = 0;
		const uint32_t px_per_m_y         = 0;
		const uint32_t colors_used        = 0;
		const uint32_t important_colors   = 0;
	} __attribute__((packed)) DIB_header(width, height);
	image.write(std::bit_cast<const char *>(&DIB_header),
	            sizeof(DIB_header));
}

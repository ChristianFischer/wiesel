/*
 * android_filesupport_png.cpp
 *
 *  Created on: 08.04.2012
 *      Author: Baldur
 */
#ifdef __ANDROID__

#include "../android_engine.h"
#include "wiesel/util/log.h"
#include <png.h>

using namespace wiesel;


bool AndroidEngine::decodeImage_PNG(
		const std::string &filename,
		unsigned char **pBuffer, size_t *pSize,  unsigned int *pWidth, unsigned int *pHeight,
		int *pRbits, int *pGbits, int *pBbits, int *pAbits,
		bool as_texture
) {
	png_structp		png_ptr;
	png_infop		info_ptr;
	unsigned int	sig_read = 0;
	int				bit_depth;
	int				color_type;
	int				interlace_type;
	png_uint_32		width;
	png_uint_32		height;
	FILE*			fp;

	// open the file
	if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
		Log::err << "file " << filename << " not found!" << std::endl;
		return false;
	}

	// Create and initialize the png_struct with the desired error handler functions.
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	// Allocate/initialize the memory for image information.
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

	png_init_io(png_ptr, fp);

	// read png
	// PNG_TRANSFORM_EXPAND:      perform set_expand()
	// PNG_TRANSFORM_PACKING:     expand 1, 2 and 4-bit samples to bytes
	// PNG_TRANSFORM_STRIP_16:    strip 16-bit samples to 8 bits
	// PNG_TRANSFORM_GRAY_TO_RGB: expand grayscale samples to RGB (or GA to RGBA)
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_GRAY_TO_RGB, 0);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, 0, 0);

	// init image info
	bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA) ? true : false;
	int bytesPerPixel = has_alpha ? 4 : 3;

	// allocate memory and read data
	unsigned char *image_data = reinterpret_cast<unsigned char*>(malloc(width * height * bytesPerPixel));
	if (image_data == NULL) {
		return false;
	}

	png_bytep *png_rows = png_get_rows(png_ptr, info_ptr);

	// copy data to image info
	int bytesPerRow = width * bytesPerPixel;
	for (unsigned int row=0; row<height; ++row) {
		memcpy(image_data + (row * bytesPerRow), png_rows[row], bytesPerRow);
	}

	// copy data into output pointers
	if (pRbits)  *pRbits  = bit_depth;
	if (pGbits)  *pGbits  = bit_depth;
	if (pBbits)  *pBbits  = bit_depth;
	if (pAbits)  *pAbits  = has_alpha ? bit_depth : 0;
	if (pWidth)  *pWidth  = width;
	if (pHeight) *pHeight = height;
	if (pSize)   *pSize   = width * height * bytesPerPixel;

	if  (pBuffer) {
		*pBuffer = image_data;
	}
	else {
		// when no buffer-pointer was provided, delete the image data
		free(image_data);
	}

	// clear the png data
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// close the file
	fclose(fp);

	return true;
}


#endif // __ANDROID__

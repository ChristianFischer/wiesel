/*
 * android_filesupport_png.cpp
 *
 *  Created on: 08.04.2012
 *      Author: Baldur
 */
#ifdef __ANDROID__

#include "../android_engine.h"
#include "wiesel/io/file.h"
#include "wiesel/util/log.h"
#include <png.h>

using namespace wiesel;


struct PNG_BufferObject
{
	DataBuffer *buffer;
	size_t offset;
};


// callback method to feed the PNG decoder with data from our buffer.
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
	PNG_BufferObject *buffer = (PNG_BufferObject*)png_get_io_ptr(png_ptr);
	size_t buffer_size = buffer->buffer->getSize();

	// fail, when reading beyond buffer
	if ((buffer->offset + length) > buffer_size) {
	//	length = buffer_size - buffer->offset;
		png_error(png_ptr, "pngReaderCallback failed");
	}

	const unsigned char *buffer_data = buffer->buffer->getData() + buffer->offset;
	memcpy(data, buffer_data, length);
	buffer->offset += length;
}



bool AndroidEngine::decodeImage_PNG(
		DataSource *data, unsigned char **pBuffer,
		size_t *pSize, unsigned int *pWidth, unsigned int *pHeight, int *pRbits,
		int *pGbits, int *pBbits, int *pAbits, bool as_texture) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int bit_depth;
	int color_type;
	int interlace_type;
	png_uint_32 width;
	png_uint_32 height;
	FILE* fp = NULL;

	// Create and initialize the png_struct with the desired error handler functions.
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		return false;
	}

	// Allocate/initialize the memory for image information.
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	bool io_initialized = false;
	FileDataSource *filedata = dynamic_cast<FileDataSource*>(data);
	if (filedata) {
		File *file = filedata->getFile();

		// open the file
		if ((fp = fopen(file->getFullPath().c_str(), "rb")) != NULL) {
			png_init_io(png_ptr, fp);

			// success
			io_initialized = true;
		}

		// maybe the file could not be opened, because it's not from a real file-system,
		// so try the buffer-access now
	}

	if (io_initialized == false) {
		PNG_BufferObject readbuffer;
		readbuffer.buffer = data->getDataBuffer();
		readbuffer.offset = 0;
        png_set_read_fn(png_ptr, &readbuffer, pngReadCallback);

        io_initialized = true;
	}

	if (io_initialized == false) {
		Log::info << "loading from source failed" << std::endl;
		return false;
	}

	// read png
	// PNG_TRANSFORM_EXPAND:      perform set_expand()
	// PNG_TRANSFORM_PACKING:     expand 1, 2 and 4-bit samples to bytes
	// PNG_TRANSFORM_STRIP_16:    strip 16-bit samples to 8 bits
	// PNG_TRANSFORM_GRAY_TO_RGB: expand grayscale samples to RGB (or GA to RGBA)
	png_read_png(png_ptr, info_ptr,
			PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_PACKING
					| PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_GRAY_TO_RGB, 0);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
			&interlace_type, 0, 0);

	// init image info
	bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA) ? true : false;
	int bytesPerPixel = has_alpha ? 4 : 3;

	// allocate memory and read data
	unsigned char *image_data = reinterpret_cast<unsigned char*>(malloc(
			width * height * bytesPerPixel));
	if (image_data == NULL) {
		return false;
	}

	png_bytep *png_rows = png_get_rows(png_ptr, info_ptr);

	// copy data to image info
	int bytesPerRow = width * bytesPerPixel;
	for (unsigned int row = 0; row < height; ++row) {
		memcpy(image_data + (row * bytesPerRow), png_rows[row], bytesPerRow);
	}

	// copy data into output pointers
	if (pRbits)
		*pRbits = bit_depth;
	if (pGbits)
		*pGbits = bit_depth;
	if (pBbits)
		*pBbits = bit_depth;
	if (pAbits)
		*pAbits = has_alpha ? bit_depth : 0;
	if (pWidth)
		*pWidth = width;
	if (pHeight)
		*pHeight = height;
	if (pSize)
		*pSize = width * height * bytesPerPixel;

	if (pBuffer) {
		*pBuffer = image_data;
	} else {
		// when no buffer-pointer was provided, delete the image data
		free(image_data);
	}

	// clear the png data
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// close the file (if any opened)
	if (fp) {
		fclose(fp);
	}

	return true;
}

#endif // __ANDROID__

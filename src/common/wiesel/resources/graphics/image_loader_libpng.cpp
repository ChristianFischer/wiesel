/**
 * Copyright (C) 2012
 * Christian Fischer
 *
 * https://bitbucket.org/baldur/wiesel/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#include "image_loader_libpng.h"

#include "wiesel/resources/graphics/imageutils.h"

#if WIESEL_SUPPORTS_LIBPNG

#include "wiesel/util/log.h"
#include <inttypes.h>
#include <malloc.h>
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



LibPngImageLoader *LibPngImageLoader::create() {
	return new LibPngImageLoader();
}

LibPngImageLoader::LibPngImageLoader() {
	return;
}

LibPngImageLoader::~LibPngImageLoader() {
	return;
}



Image *LibPngImageLoader::loadImage(DataSource *source) {
	return internal_loadImage(source, NULL, false);
}


Image *LibPngImageLoader::loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size) {
	return internal_loadImage(source, pOriginal_size, true);
}


Image *LibPngImageLoader::internal_loadImage(DataSource *source, dimension *pOriginalSize, bool pot) {
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth;
	int color_type;
	int interlace_type;
	png_uint_32 width;
	png_uint_32 height;
	png_uint_32 original_width;
	png_uint_32 original_height;
	FILE* fp = NULL;

	// Create and initialize the png_struct with the desired error handler functions.
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		return NULL;
	}

	// Allocate/initialize the memory for image information.
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}

	bool io_initialized = false;
	FileDataSource *filedata = dynamic_cast<FileDataSource*>(source);
	if (filedata) {
		File *file = filedata->getFile();

		// open the file
		if ((fp = fopen(file->getFullPath().c_str(), "rb")) != NULL) {
			// get first bytes and check if the file is a valid PNG file
			uint64_t header;
			if (
					fread(&header, 1, 8, fp) != 8
				||	png_sig_cmp(reinterpret_cast<png_byte*>(&header), 0, 8) != 0
			) {
				return NULL;
			}

			// initialize png read struct
			png_init_io(png_ptr, fp);

			// success
			io_initialized = true;
		}

		// maybe the file could not be opened, because it's not from a real file-system,
		// so try the buffer-access now
	}

	if (io_initialized == false) {
		DataBuffer *buffer = source->getDataBuffer();

		// we need to cheat, because some PNG headers seems to require non-const data
		png_bytep data_bytes = const_cast<png_bytep>(buffer->getData());

		// check, if PNG file is valid
		if (
				buffer->getSize() < 8
			||	png_sig_cmp(data_bytes, 0, 8) != 0
		) {
			return NULL;
		}

		PNG_BufferObject readbuffer;
		readbuffer.buffer = buffer;
		readbuffer.offset = 0;
		png_set_read_fn(png_ptr, &readbuffer, pngReadCallback);

		io_initialized = true;
	}

	if (io_initialized == false) {
		return NULL;
	}

	// read png
	// PNG_TRANSFORM_EXPAND:      perform set_expand()
	// PNG_TRANSFORM_PACKING:     expand 1, 2 and 4-bit samples to bytes
	// PNG_TRANSFORM_STRIP_16:    strip 16-bit samples to 8 bits
	// PNG_TRANSFORM_GRAY_TO_RGB: expand grayscale samples to RGB (or GA to RGBA)
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_GRAY_TO_RGB, 0);

	png_get_IHDR(png_ptr, info_ptr, &original_width, &original_height, &bit_depth, &color_type, &interlace_type, 0, 0);

	// when we're loading a texture, we may need a power-of-two size
	if (pot) {
		width  = getNextPowerOfTwo(original_width);
		height = getNextPowerOfTwo(original_height);
	}
	else {
		width  = original_width;
		height = original_height;
	}

	// init image info
	bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA) ? true : false;
	int bytesPerPixel = has_alpha ? 4 : 3;
	PixelFormat pixel_format;

	// determine pixelformat
	if (has_alpha) {
		pixel_format = PixelFormat_RGBA_8888;
	}
	else {
		pixel_format = PixelFormat_RGB_888;
	}

	// allocate memory and read data
	unsigned char *image_data = reinterpret_cast<unsigned char*>(malloc(width * height * bytesPerPixel));
	if (image_data == NULL) {
		return NULL;
	}

	png_bytep *png_rows = png_get_rows(png_ptr, info_ptr);

	// copy data to image info
	int bytesPerRowSrc = original_width * bytesPerPixel;
	int bytesPerRowDst = width * bytesPerPixel;
	int gap            = bytesPerRowDst - bytesPerRowSrc;
	for(unsigned int row=0; row<original_height; ++row) {
		// copy the entire line
		memcpy(image_data + (row * bytesPerRowDst), png_rows[row], bytesPerRowSrc);

		// fill the gap to the next line with transparent black color
		memset(image_data + (row * bytesPerRowDst) + bytesPerRowSrc, 0x00, gap);
	}

	// fill empty space of resized area
	memset(
			image_data + (original_height * bytesPerRowDst),
			0x00,
			(height - original_height) * bytesPerRowDst
	);

	// create the image object
	Image *image = new Image(
			new ExclusiveDataBuffer(image_data, width * height * bytesPerPixel),
			pixel_format,
			dimension(width, height)
	);

	// store original size if requested
	if (pOriginalSize) {
		pOriginalSize->width  = original_width;
		pOriginalSize->height = original_height;
	}

	// clear the png data
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// close the file (if any opened)
	if (fp) {
		fclose(fp);
	}

	return image;
}

#endif // WIESEL_SUPPORTS_LIBPNG


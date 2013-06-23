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
#include "image.h"
#include "imageutils.h"
#include <assert.h>
#include <string.h>


using namespace wiesel;



Image::Image() {
	this->pixel_data = NULL;
}


Image::Image(DataBuffer* data, PixelFormat format, const dimension& size) {
	this->pixel_data = NULL;

	bool assigned = assignImageData(data, format, size);
	assert(assigned);

	return;
}


Image::~Image() {
	clear_ref(pixel_data);
}


bool Image::assignImageData(DataBuffer* data, PixelFormat format, const dimension& size) {
	size_t new_image_size = getBytesPerPixel(format) * size.width * size.height;
	assert(new_image_size == data->getSize());

	if (new_image_size == data->getSize()) {
		clear_ref(this->pixel_data);

		this->pixel_data   = keep(data);
		this->pixel_format = format;
		this->image_size   = size;
	}

	return true;
}


bool Image::changePixelFormat(PixelFormat new_pixel_format) {
	// not supported yet.
	return false;
}


bool Image::resize(const dimension &new_image_size) {
	// oops - we got an empty buffer...
	if (pixel_data == NULL || pixel_data->getData() == NULL) {
		return false;
	}

	// both dimensions will remain the same, so we have nothing to do
	if (image_size == new_image_size) {
		return true;
	}

	// an uncommon case, one dimension will be increased, the other decreased
	// we have to do this in two steps
	if (
			((new_image_size.width > image_size.width) && (new_image_size.height < image_size.height))
		||	((new_image_size.width < image_size.width) && (new_image_size.height > image_size.height))
	) {
		bool success = false;
		success &= resize(dimension(new_image_size.width, image_size.height));
		success &= resize(dimension(new_image_size.width, new_image_size.height));
		return success;
	}

	// prepare some attributes...
	size_t bytesPerPixel = getBytesPerPixel(getPixelFormat());
	unsigned int old_width  = static_cast<unsigned int>(image_size.width);
	unsigned int old_height = static_cast<unsigned int>(image_size.height);
	unsigned int new_width  = static_cast<unsigned int>(new_image_size.width);
	unsigned int new_height = static_cast<unsigned int>(new_image_size.height);

	// increase the image's size
	if ((new_width >= old_width) && (new_height >= old_height)) {
		size_t old_line_length	= old_width * bytesPerPixel;
		size_t new_line_length	= new_width * bytesPerPixel;
	//	size_t old_size			= old_line_length * old_height;
		size_t new_size			= new_line_length * new_height;
		size_t gap				= new_line_length - old_line_length;

		// resize the buffer to it's new size
		bool resize_successfull = pixel_data->resize(new_size);

		// check, if reallocating the buffer failed
		if (resize_successfull == false) {
			return NULL;
		}

		// get pointers to required offsets.
		DataBuffer::mutable_data_t buffer = pixel_data->getMutableData();
		unsigned char *ptr_old_line = buffer + (bytesPerPixel * old_width * old_height);
		unsigned char *ptr_new_line = buffer + (bytesPerPixel * new_width * old_height);
		unsigned char *ptr_buf_end  = buffer + new_size;

		// fill remaining space to the end of the buffer
		memset(ptr_new_line, 0x00, (ptr_buf_end - ptr_new_line));

		// re-arrange the lines
		do {
			// fill the gap to the next line with zero, which should result in transparent, black pixels
			for(int cnt=gap; --cnt>=0;) {
				*(--ptr_new_line) = 0x00;
			}

			// copy the line to it's new position
			for(int cnt=old_line_length; --cnt>=0;) {
				*(--ptr_new_line) = *(--ptr_old_line);
			}
		}
		while(ptr_old_line > buffer);

		return true;
	}

	// increase the image's size
	if ((new_width <= old_width) && (new_height <= old_height)) {
		size_t old_line_length	= old_width * bytesPerPixel;
		size_t new_line_length	= new_width * bytesPerPixel;
	//	size_t old_size			= old_line_length * old_height;
		size_t new_size			= new_line_length * new_height;
		size_t gap				= old_line_length - new_line_length;

		DataBuffer::mutable_data_t buffer = pixel_data->getMutableData();
		unsigned char *ptr_old_line = buffer - 1;
		unsigned char *ptr_new_line = buffer - 1;
		unsigned char *ptr_buf_end  = buffer + new_size;

		// re-arrange the lines
		do {
			// copy the line to it's new position
			for(int cnt=new_line_length; --cnt>=0;) {
				*(++ptr_new_line) = *(++ptr_old_line);
			}

			// skip the gap
			ptr_old_line += gap;
		}
		while(ptr_new_line < ptr_buf_end);

		// finally, shrink the buffer to it's new size
		bool resize_successfull = pixel_data->resize(new_size);

		// check, if reallocating the buffer failed
		if (resize_successfull == false) {
			return false;
		}

		return true;
	}

	// did we have forgotten a single case?
	assert(false);

	return false;
}


bool Image::ensurePowerOfTwo() {
	// current size as integer
	int cur_width  = static_cast<int>(image_size.width);
	int cur_height = static_cast<int>(image_size.width);

	// compute the next pot-size
	int pot_width  = getNextPowerOfTwo(cur_width);
	int pot_height = getNextPowerOfTwo(cur_height);

	if ((cur_width != pot_width) || (cur_height != pot_height)) {
		return resize(dimension(pot_width, pot_height));
	}

	return true;
}

/*
 * imageutils.cpp
 *
 *  Created on: 16.04.2012
 *      Author: Baldur
 */
#include "imageutils.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>


using namespace wiesel;
using namespace wiesel::imageutils;


unsigned int imageutils::getNextPowerOfTwo(unsigned int num) {
	unsigned int out_num = 1;
	for(; out_num<num; out_num<<=1) { }

	return out_num;
}



unsigned char* imageutils::resizeImage(
		unsigned char *buffer,		unsigned char bytesPerPixel,
		unsigned int old_width,		unsigned int old_height,
		unsigned int new_width,		unsigned int new_height
) {
	// oops - we got an empty buffer...
	if (buffer == NULL) {
		return NULL;
	}

	// both dimensions will remain the same, so we have nothing to do
	if ((new_width == old_width) && (new_height == old_height)) {
		return buffer;
	}

	// an uncommon case, one dimension will be increased, the other decreased
	// we have to do this in two steps
	if (
			((new_width > old_width) && (new_height < old_height))
		||	((new_width < old_width) && (new_height > old_height))
	) {
		buffer = resizeImage(buffer, bytesPerPixel, old_width, old_height, new_width, old_height);
		buffer = resizeImage(buffer, bytesPerPixel, new_width, old_height, new_width, new_height);
		return buffer;
	}

	// increase the image's size
	if ((new_width >= old_width) && (new_height >= old_height)) {
		size_t old_line_length	= old_width * bytesPerPixel;
		size_t new_line_length	= new_width * bytesPerPixel;
		size_t old_size			= old_line_length * old_height;
		size_t new_size			= new_line_length * new_height;
		size_t gap				= new_line_length - old_line_length;

		buffer = reinterpret_cast<unsigned char*>(realloc(buffer, new_size));
		unsigned char *ptr_old_line = buffer + (bytesPerPixel * old_width * old_height);
		unsigned char *ptr_new_line = buffer + (bytesPerPixel * new_width * old_height);
		unsigned char *ptr_buf_end  = buffer + new_size;

		// check, if reallocating the buffer failed
		if (buffer == NULL) {
			return NULL;
		}

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

		return buffer;
	}

	// increase the image's size
	if ((new_width <= old_width) && (new_height <= old_height)) {
		size_t old_line_length	= old_width * bytesPerPixel;
		size_t new_line_length	= new_width * bytesPerPixel;
		size_t old_size			= old_line_length * old_height;
		size_t new_size			= new_line_length * new_height;
		size_t gap				= old_line_length - new_line_length;

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
		buffer = reinterpret_cast<unsigned char*>(realloc(buffer, new_size));

		// check, if reallocating the buffer failed
		if (buffer == NULL) {
			return NULL;
		}

		return buffer;
	}

	// did we have forgotten a single case?
	assert(false);

	return buffer;
}

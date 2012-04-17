/*
 * imageutils.h
 *
 *  Created on: 16.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_UTIL_IMAGEUTILS_H__
#define __WIESEL_UTIL_IMAGEUTILS_H__

namespace wiesel {
namespace imageutils {

	/**
	 * @brief compute the next power-of-two size, which is equal or greater than the input number.
	 */
	unsigned int getNextPowerOfTwo(unsigned int number);

	/**
	 * @brief increase or decrease the size of the image.
	 * @return the buffer of the resized image, which may be the same as the original input.
	 */
	unsigned char* resizeImage(
			unsigned char *buffer,			unsigned char bytesPerPixel,
			unsigned int original_width,	unsigned int original_height,
			unsigned int new_width,			unsigned int new_height
	);
}
}


#endif /* IMAGEUTILS_H_ */

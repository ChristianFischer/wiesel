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
#ifndef __WIESEL_RESOURCES_GRAPHICS_IMAGEUTILS_H__
#define __WIESEL_RESOURCES_GRAPHICS_IMAGEUTILS_H__

#include <wiesel/wiesel-core.def>

#include "image.h"


namespace wiesel {

	/**
	 * @brief compute the next power-of-two size, which is equal or greater than the input number.
	 */
	WIESEL_CORE_EXPORT unsigned int getNextPowerOfTwo(unsigned int number);

	/**
	 * @brief Get the size in bytes for a pixel in a specific pixel format.
	 */
	WIESEL_CORE_EXPORT size_t getBytesPerPixel(PixelFormat pixel_format);
}


#endif /* __WIESEL_RESOURCES_GRAPHICS_IMAGEUTILS_H__ */

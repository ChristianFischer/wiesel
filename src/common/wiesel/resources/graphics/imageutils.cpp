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
#include "imageutils.h"


namespace wiesel {

	unsigned int getNextPowerOfTwo(unsigned int num) {
		unsigned int out_num = 1;
		for(; out_num<num; out_num<<=1) { }

		return out_num;
	}



	size_t getBytesPerPixel(PixelFormat pixel_format) {
		switch(pixel_format) {
			case PixelFormat_RGB_888: {
				return 3;
			}

			case PixelFormat_RGBA_8888: {
				return 4;
			}
		}

		return 0;
	}

} // namespace wiesel


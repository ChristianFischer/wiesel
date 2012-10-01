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
#include "utils.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>

#include <cmath>
#include <cstdlib>


namespace wiesel {


	float WIESEL_BASE_EXPORT deg2rad(float deg) {
		return deg * M_PI / 180.0f;
	}


	float WIESEL_BASE_EXPORT rad2deg(float rad) {
		return rad * 180.0f / M_PI;
	}



	/**
	 * @see http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm
	 */
	template<>
	bool WIESEL_BASE_EXPORT areFloatsAlmostEqual<float>(float a, float b, unsigned int max_ulp) {
		assert(max_ulp > 0 && max_ulp < 0x400000);

		// quick test
		if (a == b) {
			return true;
		}

		// std::numeric_limits::infinity()

		// convert the first value into a twos-complement int
		int32_t aInt = *(int32_t*)&a;
		if (aInt < 0) {
			aInt = 0x80000000 - aInt;
		}

		// convert the second value into a twos-complement int
		int32_t bInt = *(int32_t*)&b;
		if (bInt < 0) {
			bInt = 0x80000000 - bInt;
		}

		uint32_t diff = std::abs(aInt - bInt);
		if (diff <= max_ulp) {
			return true;
		}

		return false;
	}


	/**
	 * @see http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm
	 */
	template<>
	bool WIESEL_BASE_EXPORT areFloatsAlmostEqual<double>(double a, double b, unsigned int max_ulp) {
		assert(max_ulp > 0 && max_ulp < 0x400000);

		// quick test
		if (a == b) {
			return true;
		}

		// std::numeric_limits::infinity()

		// convert the first value into a twos-complement int
		int64_t aInt = *(int64_t*)&a;
		if (aInt < 0) {
			aInt = 0x80000000 - aInt;
		}

		// convert the second value into a twos-complement int
		int64_t bInt = *(int64_t*)&b;
		if (bInt < 0) {
			bInt = 0x80000000 - bInt;
		}

		uint64_t diff = std::abs(aInt - bInt);
		if (diff <= max_ulp) {
			return true;
		}

		return false;
	}

}

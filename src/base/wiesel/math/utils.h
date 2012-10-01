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
#ifndef __WIESEL_MATH_UTILS_H__
#define	__WIESEL_MATH_UTILS_H__

#include <wiesel/wiesel-base.def>



namespace wiesel {

	/**
	 * @brief converts a degree angle into radians.
	 */
	float deg2rad(float deg);

	/**
	 * @brief converts a radians angle into degrees.
	 */
	float rad2deg(float rad);

	/**
	 * @brief A helper class to compare if two floats are almost equal.
	 * Since floats doesn't have
	 */
	template<typename FLOAT>
	WIESEL_BASE_EXPORT bool areFloatsAlmostEqual(FLOAT a, FLOAT b, unsigned int max_ulp=5);

}
#endif	/* __WIESEL_MATH_VECTOR2D_H__ */


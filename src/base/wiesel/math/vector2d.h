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
#ifndef __WIESEL_MATH_VECTOR2D_H__
#define	__WIESEL_MATH_VECTOR2D_H__

#include "matrix.h"

#include <ostream>


namespace wiesel {

	/**
	 * @brief A class handling a two dimensional vector.
	 */
	class vector2d
	{
	public:
		/// creates a new uninitialised vector
		vector2d();

		/// creates a copy of an existing vector
		vector2d(const vector2d& orig);

		/// creates a new vector with two float values
		vector2d(float x, float y);

		/// creates a new vector with a array of (at least) two floats
		explicit vector2d(const float *v);

		/// destructor
		~vector2d();


	// comparison operators
	public:
		const vector2d& operator=(const vector2d &other);

		bool operator ==(const vector2d &other) const;
		bool operator !=(const vector2d &other) const;

	// casting
	public:
		/// cast into a pointer to all matrix values
		inline operator const float*() const {
			return static_cast<const float*>(vec);
		}

	public:
		union {
			/// an array containing all three coordinates of this vector
			float	vec[2];

			/// a struct containing each single coordinate of this vector
			struct {
				float	x;
				float	y;
			};

			/// alias names for x and y
			struct {
				float	u;		//!< same as x
				float	v;		//!< same as y
			};
		};

	public:
		/// a null-vector
		static vector2d zero;
	};



	vector2d operator +(const vector2d &a, const vector2d &b);
	vector2d operator -(const vector2d &a, const vector2d &b);
	vector2d operator *(const vector2d &a, float s);
	vector2d operator /(const vector2d &a, float s);

	const vector2d& operator +=(vector2d &a, const vector2d &b);
	const vector2d& operator -=(vector2d &a, const vector2d &b);
	const vector2d& operator *=(vector2d &a, float s);
	const vector2d& operator /=(vector2d &a, float s);


	vector2d operator *(const vector2d &v, const matrix4x4 &m);
	vector2d operator /(const vector2d &v, const matrix4x4 &m);

	const vector2d& operator *=(vector2d &v, const matrix4x4 &m);
	const vector2d& operator /=(vector2d &v, const matrix4x4 &m);


	std::ostream& operator <<(std::ostream &o, const vector2d &v);

}
#endif	/* __WIESEL_MATH_VECTOR2D_H__ */


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
#ifndef __WIESEL_MATH_VECTOR3D_H__
#define	__WIESEL_MATH_VECTOR3D_H__

#include <wiesel/wiesel-base.def>

#include "vector2d.h"
#include "matrix.h"

#include <ostream>


namespace wiesel {

	/**
	 * @brief A class handling a three dimensional vector.
	 */
	class WIESEL_BASE_EXPORT vector3d
	{
	public:
		/// creates a new uninitialised vector
		vector3d();

		/// creates a copy of an existing vector
		vector3d(const vector3d& orig);

		/// creates a new vector with three float values
		vector3d(float x, float y, float z=.0f);

		/// creates a new vector with a array of (at least) three floats
		explicit vector3d(const float *v);

		/// creates a new three dimensional vector with the content of an existing two dimensional vector
		explicit vector3d(const vector2d &v, float z=.0f);

		/// destructor
		~vector3d();


	// operators
	public:
		const vector3d& operator=(const vector3d &other);

		bool operator ==(const vector3d &other) const;
		bool operator !=(const vector3d &other) const;

	// casting
	public:
		/// cast into a pointer to all matrix values
		inline operator const float*() const {
			return static_cast<const float*>(vec);
		}

	public:
		union {
			/// an array containing all three coordinates of this vector
			float	vec[3];

			/// a struct containing each single coordinate of this vector
			struct {
				float	x;
				float	y;
				float	z;
			};
		};

	public:
		/// a null-vector
		static WIESEL_BASE_EXPORT vector3d zero;
	};



	/**
	 * @brief Compute the dot-product or scalar-product of two vectors.
	 */
	WIESEL_BASE_EXPORT float dot(const vector3d &a, const vector3d &b);

	/**
	 * @brief Compute the cross-product of two vectors.
	 */
	WIESEL_BASE_EXPORT vector3d cross(const vector3d &a, const vector3d &b);



	WIESEL_BASE_EXPORT vector3d operator +(const vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT vector3d operator -(const vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT vector3d operator *(const vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT vector3d operator /(const vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT vector3d operator *(const vector3d &a, float s);
	WIESEL_BASE_EXPORT vector3d operator /(const vector3d &a, float s);

	WIESEL_BASE_EXPORT const vector3d& operator +=(vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT const vector3d& operator -=(vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT const vector3d& operator *=(vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT const vector3d& operator /=(vector3d &a, const vector3d &b);
	WIESEL_BASE_EXPORT const vector3d& operator *=(vector3d &a, float s);
	WIESEL_BASE_EXPORT const vector3d& operator /=(vector3d &a, float s);


	WIESEL_BASE_EXPORT vector3d operator *(const vector3d &v, const matrix4x4 &m);
	WIESEL_BASE_EXPORT vector3d operator /(const vector3d &v, const matrix4x4 &m);

	WIESEL_BASE_EXPORT const vector3d& operator *=(vector3d &v, const matrix4x4 &m);
	WIESEL_BASE_EXPORT const vector3d& operator /=(vector3d &v, const matrix4x4 &m);


	WIESEL_BASE_EXPORT std::ostream& operator <<(std::ostream &o, const vector3d &v);

}
#endif	/* __WIESEL_MATH_VECTOR3D_H__ */


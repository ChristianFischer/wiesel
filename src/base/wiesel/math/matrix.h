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
#ifndef __WIESEL_MATH_MATRIX_H__
#define	__WIESEL_MATH_MATRIX_H__

#include <wiesel/wiesel-base.def>

#include <ostream>


namespace wiesel {

	/**
	 * @brief A class covering a 4x4 matrix for 3D transformation.
	 */
	class WIESEL_BASE_EXPORT matrix4x4
	{
	public:
		/// creates a new uninitialised matrix.
		matrix4x4() {}

		/// creates a copy of a existing matrix
		matrix4x4(const matrix4x4 &other);

		/// creates a matrix with initialising float values
		explicit matrix4x4(const float *m);

		/// creates a new matrix with 4x4 values
		matrix4x4(
				float m11,	float m12,	float m13,	float m14,
				float m21,	float m22,	float m23,	float m24,
				float m31,	float m32,	float m33,	float m34,
				float m41,	float m42,	float m43,	float m44
		);

		/// destructor
		~matrix4x4() {};

	public:
		/**
		 * @brief Creates a matrix for orthographical projection.
		 * @param left		Left border of the viewport.
		 * @param right		Right border of the viewport.
		 * @param bottom	Bottom border of the viewport.
		 * @param top		Top border of the viewport.
		 * @param near		Near clipping plane.
		 * @param far		Far clipping plane.
		 * @return
		 */
		static matrix4x4 ortho(float left, float right, float bottom, float top, float near, float far);

		/**
		 * @brief Creates a matrix for perspective projection.
		 * @param left		Left border of the viewport.
		 * @param right		Right border of the viewport.
		 * @param bottom	Bottom border of the viewport.
		 * @param top		Top border of the viewport.
		 * @param near		Near clipping plane.
		 * @param far		Far clipping plane.
		 * @return
		 */
		static matrix4x4 frustum(float left, float right, float bottom, float top, float near, float far);

	public:
		/// get any cell from the matrix
		inline float get(unsigned int c, unsigned int r) const {
			return m[c + r*4];
		}

		/// compute the determinant
		float det() const;

		/// reset this matrix into identity matrix
		inline void setIdentity() {
			*this = identity;
		}

		/// check, if this matrix is a identity matrix
		bool isIdentity() const {
			return *this == identity;
		}

		/// get the inverted matrix
		matrix4x4 inverted() const;


	// operations
	public:
		/// translate the matrix on x- and y-axis.
		void translate(float x, float y);

		/// translate the matrix on x-, y- and z-axis.
		void translate(float x, float y, float z);

		/// rotate on x-axis
		void rotateX(float a);

		/// rotate on y-axis
		void rotateY(float a);

		/// rotate on z-axis
		void rotateZ(float a);

		/// scale the matrix on x-, y- and z-axis.
		void scale(float x, float y, float z);

		/// scale on x-axis
		void scaleX(float s);

		/// scale on y-axis
		void scaleY(float s);

		/// scale on z-axis
		void scaleZ(float s);

	// tests
	public:
		/**
		 * @brief Tests, if this matrix equals an other.
		 * Unlike the comparision operator, this function allows a small difference,
		 * since floating operations will produce rounding errors, so comparing
		 * floats needs a bit of tolerance.
		 */
		bool equals(const matrix4x4 &other, int tolerance=5) const;

	// operators
	public:
		const matrix4x4& operator=(const matrix4x4 &other);

		bool operator ==(const matrix4x4 &other) const;
		bool operator !=(const matrix4x4 &other) const;

		matrix4x4& operator *=(const matrix4x4 &other);
		matrix4x4& operator /=(const matrix4x4 &other);


	// casting
	public:
		/// cast into a pointer to all matrix values
		inline operator const float*() const {
			return static_cast<const float*>(m);
		}

	public:
		union {
			/// an array including all the matrix fields
			float	m[16];

			/// a struct containing each single cell of this matrix
			struct {
				float	m11,	m12,	m13,	m14;
				float	m21,	m22,	m23,	m24;
				float	m31,	m32,	m33,	m34;
				float	m41,	m42,	m43,	m44;
			};
		};

	public:
		/// the identity matrix
		static WIESEL_BASE_EXPORT const matrix4x4 identity;

		/// the zero matrix
		static WIESEL_BASE_EXPORT const matrix4x4 zero;
	};




	WIESEL_BASE_EXPORT matrix4x4 operator *(const matrix4x4 &a, const matrix4x4 &b);
	WIESEL_BASE_EXPORT matrix4x4 operator /(const matrix4x4 &a, const matrix4x4 &b);


	WIESEL_BASE_EXPORT std::ostream& operator <<(std::ostream &o, const matrix4x4 &m);

}
#endif	/* __WIESEL_MATH_MATRIX_H__ */


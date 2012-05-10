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
#include "matrix.h"

#include <assert.h>
#include <math.h>
#include <string.h>


using namespace wiesel;


const matrix4x4		matrix4x4::identity = matrix4x4(
													1.0f,	0.0f,	0.0f,	0.0f,
													0.0f,	1.0f,	0.0f,	0.0f,
													0.0f,	0.0f,	1.0f,	0.0f,
													0.0f,	0.0f,	0.0f,	1.0f
										);

const matrix4x4		matrix4x4::zero = matrix4x4(
													0.0f,	0.0f,	0.0f,	0.0f,
													0.0f,	0.0f,	0.0f,	0.0f,
													0.0f,	0.0f,	0.0f,	0.0f,
													0.0f,	0.0f,	0.0f,	0.0f
										);


matrix4x4::matrix4x4(const matrix4x4 &other) {
	const float* src = other.m;
	      float* dst = this->m;
	      float* end = this->m + 16;

	while(dst != end) {
		*dst = *src;
		++src;
		++dst;
	}

	return;
}

matrix4x4::matrix4x4(const float *m) {
	const float* src = m;
	      float* dst = this->m;
	      float* end = this->m + 16;

	while(dst != end) {
		*dst = *src;
		++src;
		++dst;
	}

	return;
}

matrix4x4::matrix4x4(
				float m11,	float m12,	float m13,	float m14,
				float m21,	float m22,	float m23,	float m24,
				float m31,	float m32,	float m33,	float m34,
				float m41,	float m42,	float m43,	float m44
) :
	m11(m11),	m12(m12),	m13(m13),	m14(m14),
	m21(m21),	m22(m22),	m23(m23),	m24(m24),
	m31(m31),	m32(m32),	m33(m33),	m34(m34),
	m41(m41),	m42(m42),	m43(m43),	m44(m44)
{
	return;
}


matrix4x4 matrix4x4::ortho(float l, float r, float b, float t, float n, float f) {
	assert(l < r);
	assert(b < t);
	assert(n < f);

	return matrix4x4(
				2/(r-l),	0.0f,		0.0f,		-((r+l)/(r-l)),
				0.0f,		2/(t-b),	0.0f,		-((t+b)/(t-b)),
				0.0f,		0.0f,		(-2)/(f-n),	-((f+n)/(f-n)),
				0.0f,		0.0f,		0.0f,		 1.0f
	);
}


matrix4x4 matrix4x4::frustum(float l, float r, float b, float t, float n, float f) {
	assert(l < r);
	assert(b < t);
	assert(n < f);

	return matrix4x4(
				((2*n)/(r-l)),	0.0f,			((r+l)/(r-l)),		0.0f,
				0.0f,			((2*n)/(t-b)),	((t+b)/(t-b)),		0.0f,
				0.0f,			0.0f,			((-(f+n))/(f-n)),	((-(2*f*n))/(f-n)),
				0.0f,			0.0f,			-1.0f,				0.0f
	);

}




float matrix4x4::det() const {
	return
			(m11 * (m22 * m33 - m23 * m32))
		-	(m12 * (m21 * m33 - m23 * m31))
		+	(m14 * (m21 * m32 - m22 * m31))
	;
}


matrix4x4 matrix4x4::inverted() const {
	float inv_det = det();

	if (inv_det != 0.0f) {
		matrix4x4 result;

		// invert determinant
		inv_det = 1.0f / inv_det;

		result.m11 =  inv_det * ((m22 * m33) - (m23 * m32));
		result.m12 = -inv_det * ((m12 * m33) - (m13 * m32));
		result.m13 =  inv_det * ((m13 * m23) - (m13 * m22));
		result.m14 = 0.0f;

		result.m21 = -inv_det * ((m21 * m33) - (m23 * m31));
		result.m22 =  inv_det * ((m11 * m33) - (m13 * m31));
		result.m23 = -inv_det * ((m11 * m23) - (m13 * m21));
		result.m24 = 0.0f;

		result.m31 =  inv_det * ((m21 * m32) - (m22 * m31));
		result.m32 = -inv_det * ((m11 * m32) - (m12 * m31));
		result.m33 =  inv_det * ((m11 * m22) - (m12 * m21));
		result.m34 = 0.0f;

		result.m41 = -((m41 * result.m11) + (m42 * result.m21) + (m43 * result.m31));
		result.m42 = -((m41 * result.m12) + (m42 * result.m22) + (m43 * result.m32));
		result.m43 = -((m41 * result.m13) + (m42 * result.m23) + (m43 * result.m33));
		result.m44 = 1.0f;

		return result;
	}

	return identity;
}








void matrix4x4::translate(float x, float y) {
	*this *= matrix4x4(
						1.0f, 0.0f, 0.0f,    x,
						0.0f, 1.0f, 0.0f,    y,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);
}


void matrix4x4::translate(float x, float y, float z) {
	*this *= matrix4x4(
						1.0f, 0.0f, 0.0f,    x,
						0.0f, 1.0f, 0.0f,    y,
						0.0f, 0.0f, 1.0f,    z,
						0.0f, 0.0f, 0.0f, 1.0f
	);
}


void matrix4x4::rotateX(float a) {
	float sin = sinf(a);
	float cos = cosf(a);

	*this *= matrix4x4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, +cos, -sin, 0.0f,
						0.0f, +sin, +cos, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::rotateY(float a) {
	float sin = sinf(a);
	float cos = cosf(a);

	*this *= matrix4x4(
						+cos, 0.0f, +sin, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						-sin, 0.0f, +cos, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::rotateZ(float a) {
	float sin = sinf(a);
	float cos = cosf(a);

	*this *= matrix4x4(
						+cos, -sin, 0.0f, 0.0f,
						+sin, +cos, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::scale(float x, float y, float z) {
	*this *= matrix4x4(
						   x, 0.0f, 0.0f, 0.0f,
						0.0f,    y, 0.0f, 0.0f,
						0.0f, 0.0f,    z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::scaleX(float s) {
	*this *= matrix4x4(
						   s, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::scaleY(float s) {
	*this *= matrix4x4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f,    s, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}


void matrix4x4::scaleZ(float s) {
	*this *= matrix4x4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f,    s, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
	);

	return;
}







const matrix4x4& matrix4x4::operator =(const matrix4x4& other) {
	const float* src = other.m;
	      float* dst = this->m;
	      float* end = this->m + 16;

	while(dst != end) {
		*dst = *src;
		++src;
		++dst;
	}

	return *this;
}


bool matrix4x4::operator ==(const matrix4x4 &other) const {
	for(register int i=16; --i>=0;) {
		if (this->m[i] != other.m[i]) {
			return false;
		}
	}

	return true;
}


bool matrix4x4::operator !=(const matrix4x4 &other) const {
	for(register int i=16; --i>=0;) {
		if (this->m[i] == other.m[i]) {
			return false;
		}
	}

	return true;
}


matrix4x4& matrix4x4::operator *=(const matrix4x4 &other) {
	(*this) = (*this) * other;
	return *this;
}

matrix4x4& matrix4x4::operator /=(const matrix4x4 &other) {
	(*this) = (*this) / other;
	return *this;
}







matrix4x4 wiesel::operator *(const matrix4x4 &a, const matrix4x4 &b) {
	return matrix4x4(
			(a.m11 * b.m11) + (a.m21 * b.m12) + (a.m31 * b.m13) + (a.m41 * b.m14),
			(a.m12 * b.m11) + (a.m22 * b.m12) + (a.m32 * b.m13) + (a.m42 * b.m14),
			(a.m13 * b.m11) + (a.m23 * b.m12) + (a.m33 * b.m13) + (a.m43 * b.m14),
			(a.m14 * b.m11) + (a.m24 * b.m12) + (a.m34 * b.m13) + (a.m44 * b.m14),

			(a.m11 * b.m21) + (a.m21 * b.m22) + (a.m31 * b.m23) + (a.m41 * b.m24),
			(a.m12 * b.m21) + (a.m22 * b.m22) + (a.m32 * b.m23) + (a.m42 * b.m24),
			(a.m13 * b.m21) + (a.m23 * b.m22) + (a.m33 * b.m23) + (a.m43 * b.m24),
			(a.m14 * b.m21) + (a.m24 * b.m22) + (a.m34 * b.m23) + (a.m44 * b.m24),

			(a.m11 * b.m31) + (a.m21 * b.m32) + (a.m31 * b.m33) + (a.m41 * b.m34),
			(a.m12 * b.m31) + (a.m22 * b.m32) + (a.m32 * b.m33) + (a.m42 * b.m34),
			(a.m13 * b.m31) + (a.m23 * b.m32) + (a.m33 * b.m33) + (a.m43 * b.m34),
			(a.m14 * b.m31) + (a.m24 * b.m32) + (a.m34 * b.m33) + (a.m44 * b.m34),

			(a.m11 * b.m41) + (a.m21 * b.m42) + (a.m31 * b.m43) + (a.m41 * b.m44),
			(a.m12 * b.m41) + (a.m22 * b.m42) + (a.m32 * b.m43) + (a.m42 * b.m44),
			(a.m13 * b.m41) + (a.m23 * b.m42) + (a.m33 * b.m43) + (a.m43 * b.m44),
			(a.m14 * b.m41) + (a.m24 * b.m42) + (a.m34 * b.m43) + (a.m44 * b.m44)
	);
}


matrix4x4 wiesel::operator /(const matrix4x4 &a, const matrix4x4 &b) {
	return a * b.inverted();
}

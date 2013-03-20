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
#include "utils.h"

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
	matrix4x4 result;

	float s0 = m11 * m22 - m21 * m12;
	float s1 = m11 * m23 - m21 * m13;
	float s2 = m11 * m24 - m21 * m14;
	float s3 = m12 * m23 - m22 * m13;
	float s4 = m12 * m24 - m22 * m14;
	float s5 = m13 * m24 - m23 * m14;

	float c5 = m33 * m44 - m43 * m34;
	float c4 = m32 * m44 - m42 * m34;
	float c3 = m32 * m43 - m42 * m33;
	float c2 = m31 * m44 - m41 * m34;
	float c1 = m31 * m43 - m41 * m33;
	float c0 = m31 * m42 - m41 * m32;

	float det = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
	if (det == 0.0f) {
		result = identity;
	}
	else {
		float invdet = 1.0f / det;

		result.m11 = ( m22 * c5 - m23 * c4 + m24 * c3) * invdet;
		result.m12 = (-m12 * c5 + m13 * c4 - m14 * c3) * invdet;
		result.m13 = ( m42 * s5 - m43 * s4 + m44 * s3) * invdet;
		result.m14 = (-m32 * s5 + m33 * s4 - m34 * s3) * invdet;

		result.m21 = (-m21 * c5 + m23 * c2 - m24 * c1) * invdet;
		result.m22 = ( m11 * c5 - m13 * c2 + m14 * c1) * invdet;
		result.m23 = (-m41 * s5 + m43 * s2 - m44 * s1) * invdet;
		result.m24 = ( m31 * s5 - m33 * s2 + m34 * s1) * invdet;

		result.m31 = ( m21 * c4 - m22 * c2 + m24 * c0) * invdet;
		result.m32 = (-m11 * c4 + m12 * c2 - m14 * c0) * invdet;
		result.m33 = ( m41 * s4 - m42 * s2 + m44 * s0) * invdet;
		result.m34 = (-m31 * s4 + m32 * s2 - m34 * s0) * invdet;

		result.m41 = (-m21 * c3 + m22 * c1 - m23 * c0) * invdet;
		result.m42 = ( m11 * c3 - m12 * c1 + m13 * c0) * invdet;
		result.m43 = (-m41 * s3 + m42 * s1 - m43 * s0) * invdet;
		result.m44 = ( m31 * s3 - m32 * s1 + m33 * s0) * invdet;
	}

	return result;
}


matrix4x4 matrix4x4::transposed() const {
	return matrix4x4(
			m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44
	);
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




bool matrix4x4::equals(const matrix4x4 &other, int tolerance) const {
	for(register int i=16; --i>=0;) {
		if (!areFloatsAlmostEqual(m[i], other.m[i], tolerance)) {
			return false;
		}
	}

	return true;
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
		if (this->m[i] != other.m[i]) {
			return true;
		}
	}

	return false;
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


std::ostream& wiesel::operator <<(std::ostream &o, const matrix4x4 &m) {
	o
		<<	"{ " << m.m11 << ',' << m.m12 << ',' << m.m13 << ',' << m.m14 << ',' << std::endl
		<<	"  " << m.m21 << ',' << m.m22 << ',' << m.m23 << ',' << m.m24 << ',' << std::endl
		<<	"  " << m.m31 << ',' << m.m32 << ',' << m.m33 << ',' << m.m34 << ',' << std::endl
		<<	"  " << m.m41 << ',' << m.m42 << ',' << m.m43 << ',' << m.m44 << " }"
	;

	return o;
}

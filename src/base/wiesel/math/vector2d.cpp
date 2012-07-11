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
#include "vector2d.h"


using namespace wiesel;


vector2d vector2d::zero = vector2d(0.0f, 0.0f);


vector2d::vector2d() {
	return;
}

vector2d::vector2d(const vector2d& orig)
 :	x(orig.x),
	y(orig.y)
{
	return;
}

vector2d::vector2d(const float *v)
 :	x(v[0]),
	y(v[1])
{
	return;
}

vector2d::vector2d(float x, float y)
 :	x(x),
	y(y)
{
	return;
}

vector2d::~vector2d() {
	return;
}




const vector2d& vector2d::operator =(const vector2d &other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool vector2d::operator ==(const vector2d &other) const {
	return this->x==other.x && this->y==other.y;
}

bool vector2d::operator !=(const vector2d &other) const {
	return this->x!=other.x || this->y!=other.y;
}





vector2d wiesel::operator +(const vector2d &a, const vector2d &b) {
	return vector2d(a.x+b.x, a.y+b.y);
}

vector2d wiesel::operator -(const vector2d &a, const vector2d &b) {
	return vector2d(a.x-b.x, a.y-b.y);
}

vector2d wiesel::operator *(const vector2d &a, float s) {
	return vector2d(a.x*s, a.y*s);
}

vector2d wiesel::operator /(const vector2d &a, float s) {
	return vector2d(a.x/s, a.y/s);
}



const vector2d& wiesel::operator +=(vector2d &a, const vector2d &b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

const vector2d& wiesel::operator -=(vector2d &a, const vector2d &b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

const vector2d& wiesel::operator *=(vector2d &a, float s) {
	a.x *= s;
	a.y *= s;
	return a;
}

const vector2d& wiesel::operator /=(vector2d &a, float s) {
	a.x /= s;
	a.y /= s;
	return a;
}





vector2d wiesel::operator *(const vector2d &v, const matrix4x4 &m) {
	const float z = 0.0f;
	const float w = 1.0f;

	return vector2d(
			(v.x * m.m11) + (v.y * m.m21) + (z * m.m31) + (w * m.m41),
			(v.x * m.m12) + (v.y * m.m22) + (z * m.m32) + (w * m.m42)
	);
}

vector2d wiesel::operator /(const vector2d &v, const matrix4x4 &m) {
	return v * m.inverted();
}


const vector2d& wiesel::operator *=(vector2d &v, const matrix4x4 &m) {
	const float z = 0.0f;
	const float w = 1.0f;

	v = vector2d(
			(v.x * m.m11) + (v.y * m.m21) + (z * m.m31) + (w * m.m41),
			(v.x * m.m12) + (v.y * m.m22) + (z * m.m32) + (w * m.m42)
	);

	return v;
}

const vector2d& wiesel::operator /=(vector2d &v, const matrix4x4 &m) {
	return (v *= m.inverted());
}


std::ostream& wiesel::operator <<(std::ostream &o, const vector2d &v) {
	o << '[' << v.x << ',' << v.y << ']';
	return o;
}

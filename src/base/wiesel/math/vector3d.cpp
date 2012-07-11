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
#include "vector3d.h"


using namespace wiesel;


vector3d vector3d::zero = vector3d(0.0f, 0.0f, 0.0f);


vector3d::vector3d() {
	return;
}

vector3d::vector3d(const vector3d& orig)
 :	x(orig.x),
	y(orig.y),
	z(orig.z)
{
	return;
}

vector3d::vector3d(const float *v)
 :	x(v[0]),
	y(v[1]),
	z(v[2])
{
	return;
}

vector3d::vector3d(float x, float y, float z)
 :	x(x),
	y(y),
	z(z)
{
	return;
}

vector3d::~vector3d() {
	return;
}




const vector3d& vector3d::operator =(const vector3d &other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

bool vector3d::operator ==(const vector3d &other) const {
	return this->x==other.x && this->y==other.y && this->z==other.z;
}

bool vector3d::operator !=(const vector3d &other) const {
	return this->x!=other.x || this->y!=other.y || this->z!=other.z;
}




float wiesel::dot(const vector3d &a, const vector3d &b) {
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z));
}


vector3d wiesel::cross(const vector3d &a, const vector3d &b) {
	return vector3d(
			((a.y * b.z) - (a.z * b.y)),
			((a.z * b.x) - (a.x * b.z)),
			((a.x * b.y) - (a.y * b.x))
	);
}



vector3d wiesel::operator +(const vector3d &a, const vector3d &b) {
	return vector3d(a.x+b.x, a.y+b.y, a.z+b.z);
}

vector3d wiesel::operator -(const vector3d &a, const vector3d &b) {
	return vector3d(a.x-b.x, a.y-b.y, a.z-b.z);
}

vector3d wiesel::operator *(const vector3d &a, float s) {
	return vector3d(a.x*s, a.y*s, a.z*s);
}

vector3d wiesel::operator /(const vector3d &a, float s) {
	return vector3d(a.x/s, a.y/s, a.z/s);
}



const vector3d& wiesel::operator +=(vector3d &a, const vector3d &b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

const vector3d& wiesel::operator -=(vector3d &a, const vector3d &b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

const vector3d& wiesel::operator *=(vector3d &a, float s) {
	a.x *= s;
	a.y *= s;
	a.z *= s;
	return a;
}

const vector3d& wiesel::operator /=(vector3d &a, float s) {
	a.x /= s;
	a.y /= s;
	a.z /= s;
	return a;
}





vector3d wiesel::operator *(const vector3d &v, const matrix4x4 &m) {
	const float w = 1.0f;

	return vector3d(
			(v.x * m.m11) + (v.y * m.m21) + (v.z * m.m31) + (w * m.m41),
			(v.x * m.m12) + (v.y * m.m22) + (v.z * m.m32) + (w * m.m42),
			(v.x * m.m13) + (v.y * m.m23) + (v.z * m.m33) + (w * m.m43)
	);
}

vector3d wiesel::operator /(const vector3d &v, const matrix4x4 &m) {
	return v * m.inverted();
}


const vector3d& wiesel::operator *=(vector3d &v, const matrix4x4 &m) {
	const float w = 1.0f;

	v = vector3d(
			(v.x * m.m11) + (v.y * m.m21) + (v.z * m.m31) + (w * m.m41),
			(v.x * m.m12) + (v.y * m.m22) + (v.z * m.m32) + (w * m.m42),
			(v.x * m.m13) + (v.y * m.m23) + (v.z * m.m33) + (w * m.m43)
	);

	return v;
}

const vector3d& wiesel::operator /=(vector3d &v, const matrix4x4 &m) {
	return (v *= m.inverted());
}


std::ostream& wiesel::operator <<(std::ostream &o, const vector3d &v) {
	o << '[' << v.x << ',' << v.y << ',' << v.z << ']';
	return o;
}

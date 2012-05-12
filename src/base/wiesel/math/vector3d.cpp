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



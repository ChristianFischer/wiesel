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
#include "geometry.h"

using namespace wiesel;


dimension::dimension() 
: width(0.0f), height(0.0f) {
	return;
}

dimension::dimension(float width, float height) 
: width(width), height(height) {
	return;
}

dimension::dimension(const dimension &other) 
: width(other.width), height(other.height) {
	return;
}


void dimension::scale(float s) {
	this->width  *= s;
	this->height *= s;
}

void dimension::scale(float sx, float sy) {
	this->width  *= sx;
	this->height *= sy;
}

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

#include <algorithm>

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




float dimension::getMin() const {
	return std::min(width, height);
}

float dimension::getMax() const {
	return std::max(width, height);
}

float dimension::getAverage() const {
	return (width + height) / 2;
}

float dimension::getRatio() const {
	return width / height;
}


void dimension::scale(float s) {
	this->width  *= s;
	this->height *= s;
}

void dimension::scale(float sx, float sy) {
	this->width  *= sx;
	this->height *= sy;
}





rect::rect()
: position(vector2d::zero), size(0, 0)
{
	return;
}


rect::rect(float x, float y, float w, float h)
: position(x, y), size(w, h)
{
	return;
}


rect::rect(const vector2d &position, const dimension &size)
: position(position), size(size)
{
	return;
}


rect::rect(float width, float height)
: position(vector2d::zero), size(width, height)
{
	return;
}


rect::rect(const dimension &size)
: position(vector2d::zero), size(size)
{
	return;
}


rect::~rect() {
	return;
}


void rect::normalize() {
	if (size.width < 0) {
		position.x +=  size.width;
		size.width  = -size.width;
	}

	if (size.height < 0) {
		position.y  +=  size.height;
		size.height  = -size.height;
	}

	return;
}


rect rect::normalized() const {
	rect norm(*this);
	norm.normalize();
	return norm;
}


float rect::getMinX() const {
	return position.x;
}

float rect::getMaxX() const {
	return position.x + size.width;
}

float rect::getCenterX() const {
	return position.x + size.width/2;
}

float rect::getMinY() const {
	return position.y;
}

float rect::getMaxY() const {
	return position.y + size.height;
}

float rect::getCenterY() const {
	return position.y + size.height/2;
}



std::ostream& wiesel::operator <<(std::ostream &o, const dimension &dim) {
	o << '[' << dim.width << 'x' << dim.height << ']';
	return o;
}

std::ostream& wiesel::operator <<(std::ostream &o, const rect &r) {
	o << '[' << r.position << ',' << r.size << ']';
	return o;
}


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


const dimension& dimension::operator=(const dimension &other) {
	this->width  = other.width;
	this->height = other.height;
	return *this;
}


bool dimension::operator==(const dimension &other) const {
	return
			this->width  == other.width
		&&	this->height == other.height
	;
}

bool dimension::operator!=(const dimension &other) const {
	return
			this->width  != other.width
		||	this->height != other.height
	;
}





rectangle::rectangle()
: position(vector2d::zero), size(0, 0)
{
	return;
}


rectangle::rectangle(float x, float y, float w, float h)
: position(x, y), size(w, h)
{
	return;
}


rectangle::rectangle(const vector2d &position, const dimension &size)
: position(position), size(size)
{
	return;
}


rectangle::rectangle(float width, float height)
: position(vector2d::zero), size(width, height)
{
	return;
}


rectangle::rectangle(const dimension &size)
: position(vector2d::zero), size(size)
{
	return;
}


rectangle::~rectangle() {
	return;
}


void rectangle::normalize() {
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


rectangle rectangle::normalized() const {
	rectangle norm(*this);
	norm.normalize();
	return norm;
}


bool rectangle::isNormalized() const {
	return
			this->size.width  >= 0
		&&	this->size.height >= 0
	;
}


float rectangle::getMinX() const {
	return position.x;
}

float rectangle::getMaxX() const {
	return position.x + size.width;
}

float rectangle::getCenterX() const {
	return position.x + size.width/2;
}

float rectangle::getMinY() const {
	return position.y;
}

float rectangle::getMaxY() const {
	return position.y + size.height;
}

float rectangle::getCenterY() const {
	return position.y + size.height/2;
}


const rectangle& rectangle::operator=(const rectangle &other) {
	this->position = other.position;
	this->size     = other.size;
	return *this;
}


bool rectangle::operator==(const rectangle &other) const {
	return
			this->position == other.position
		&&	this->size     == other.size
	;
}

bool rectangle::operator!=(const rectangle &other) const {
	return
			this->position != other.position
		||	this->size     != other.size
	;
}


bool rectangle::contains(float x, float y) const {
	if (
			x >= getMinX()
		&&	x <= getMaxX()
		&&	y >= getMinY()
		&&	y <= getMaxY()
	) {
		return true;
	}

	return false;
}

bool rectangle::contains(const vector2d& v) const {
	return contains(v.x, v.y);
}


bool rectangle::contains(const rectangle& r) const {
	if (
			r.getMinX() < this->getMinX()
		||	r.getMinY() < this->getMinY()
		||	r.getMaxX() > this->getMaxX()
		||	r.getMaxY() > this->getMaxY()
	) {
		return false;
	}

	return true;
}


bool rectangle::intersects(const rectangle& r) const {
	if (
			r.getMinX() > this->getMaxX()
		||	r.getMinY() > this->getMaxY()
		||	r.getMaxX() < this->getMinX()
		||	r.getMaxY() < this->getMinY()
	) {
		// 'r' is completely outside of 'this'
		return false;
	}

	// intersection
	return true;
}



rectangle wiesel::createUnion(const rectangle &a, const rectangle &b) {
	float min_x = std::min(a.getMinX(), b.getMinX());
	float max_x = std::max(a.getMaxX(), b.getMaxX());
	float min_y = std::min(a.getMinY(), b.getMinY());
	float max_y = std::max(a.getMaxY(), b.getMaxY());

	return rectangle(
			(min_x),
			(min_y),
			(max_x - min_x),
			(max_y - min_y)
	);
}


rectangle wiesel::createIntersection(const rectangle &a, const rectangle &b) {
	if (a.intersects(b)) {
		float min_x = std::max(a.getMinX(), b.getMinX());
		float max_x = std::min(a.getMaxX(), b.getMaxX());
		float min_y = std::max(a.getMinY(), b.getMinY());
		float max_y = std::min(a.getMaxY(), b.getMaxY());

		return rectangle(
				(min_x),
				(min_y),
				(max_x - min_x),
				(max_y - min_y)
		);
	}

	return rectangle();
}



std::ostream& wiesel::operator <<(std::ostream &o, const dimension &dim) {
	o << '[' << dim.width << 'x' << dim.height << ']';
	return o;
}

std::ostream& wiesel::operator <<(std::ostream &o, const rectangle &r) {
	o << '[' << r.position << ',' << r.size << ']';
	return o;
}


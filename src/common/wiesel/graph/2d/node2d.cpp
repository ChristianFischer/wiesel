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
#include "node2d.h"

#include <math.h>


using namespace wiesel;


Node2D::Node2D()
 :
	position(vector2d::zero),
	rotation(0.0f),
	scale_x(1.0f),
	scale_y(1.0f)
{
	return;
}


Node2D::~Node2D() {
}


void Node2D::setPosition(const vector2d &v) {
	this->position = v;
	setTransformDirty();
}

void Node2D::setPosition(float x, float y) {
	this->position.x = x;
	this->position.y = y;
	setTransformDirty();
}

void Node2D::setRotation(float a) {
	this->rotation = a;
	setTransformDirty();
}

void Node2D::setScale(float s) {
	this->scale_x = s;
	this->scale_y = s;
	setTransformDirty();
}

void Node2D::setScaleX(float sx) {
	this->scale_x = sx;
	setTransformDirty();
}

void Node2D::setScaleY(float sy) {
	this->scale_y = sy;
	setTransformDirty();
}


void Node2D::updateTransform() {
	local_transform = matrix4x4::identity;

	local_transform.scale(scale_x, scale_y, 0.0f);
	local_transform.rotateZ(rotation * M_PI / 180.0f);
	local_transform.translate(position.x, position.y);

	Node::updateTransform();

	return;
}


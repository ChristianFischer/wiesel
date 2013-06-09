/**
 * Copyright (C) 2013
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
#include "light_node_2d.h"

using namespace wiesel;


LightNode2D::LightNode2D() {
	setTransformDirty();
}


LightNode2D::~LightNode2D() {
}


void LightNode2D::setLightZPosition(float z) {
	this->light_z = z;
}


vector3d LightNode2D::getWorldLocation() {
	if (isTransformDirty()) {
		updateTransform();
	}

	return vector3d(0.0f, 0.0f, light_z) * getWorldTransform();
}


void LightNode2D::onDraw(wiesel::video::RenderContext* render_context) {
	Node2D::onDraw(render_context);
}


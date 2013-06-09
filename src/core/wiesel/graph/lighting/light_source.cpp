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
#include "light_source.h"

using namespace wiesel;


LightSource::LightSource() {
	this->enabled			= true;
	this->strength			= 1.0f;
	this->color_ambient		= vector3d(1.0f, 1.0f, 1.0f);
	this->color_diffuse		= vector3d(1.0f, 1.0f, 1.0f);
	this->color_specular	= vector3d(1.0f, 1.0f, 1.0f);
	return;
}

LightSource::~LightSource() {
}


void LightSource::setLightEnabled(bool enabled) {
	this->enabled = enabled;
}


void LightSource::setLightColorAmbient(float r, float g, float b) {
	this->color_ambient = vector3d(r, g, b);
}


void LightSource::setLightColorAmbient(const vector3d& color) {
	this->color_ambient = color;
}


void LightSource::setLightColorDiffuse(float r, float g, float b) {
	this->color_diffuse = vector3d(r, g, b);
}


void LightSource::setLightColorDiffuse(const vector3d& color) {
	this->color_diffuse = color;
}


void LightSource::setLightColorSpecular(float r, float g, float b) {
	this->color_specular = vector3d(r, g, b);
}


void LightSource::setLightColorSpecular(const vector3d& color) {
	this->color_specular = color;
}


void LightSource::setLightStrength(float strength) {
	this->strength = strength;
}


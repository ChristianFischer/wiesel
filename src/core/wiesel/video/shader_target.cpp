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
#include <string.h>

#include "shader_target.h"
#include "render_context.h"

using namespace wiesel;
using namespace wiesel::video;


ShaderTarget::ShaderTarget() {
	this->shader		= NULL;
}

ShaderTarget::~ShaderTarget() {
	for(ValueMap::iterator it=values.begin(); it!=values.end(); it++) {
		delete it->second;
	}

	setShader(NULL);
}


void ShaderTarget::setShader(Shader* shader) {
	if (this->shader != shader) {
		if (this->shader) {
			this->shader->release();
			this->shader = NULL;
		}

		if (shader) {
			this->shader = shader;
			this->shader->retain();
		}
	}

	return;
}


void ShaderTarget::setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, const void *pValue) {
	ValueMap::iterator it = values.find(name);

	// when the value already exists, but has a different type, we need to delete the old one
	if (it != values.end()) {
		if (it->second->match(type, elements) == false) {
			delete it->second;
			values.erase(it);
			it = values.end();
		}
	}

	// create a new element, if none exists
	if (it == values.end()) {
		it = values.insert(std::pair<std::string,entry*>(name, new entry(type, elements))).first;
	}

	assert(it != values.end());
	assert(it->second->match(type, elements));

	// now we can assign the value
	it->second->set(pValue);

	return;
}


void ShaderTarget::setShaderValue(const std::string &name, int32_t i) {
	setShaderValue(name, Shader::TypeInt32, 1, &i);
}


void ShaderTarget::setShaderValue(const std::string &name, float f) {
	setShaderValue(name, Shader::TypeFloat, 1, &f);
}


void ShaderTarget::setShaderValue(const std::string &name, const vector2d &v) {
	setShaderValue(name, Shader::TypeVector2f, 1, (const float*)v);
}


void ShaderTarget::setShaderValue(const std::string &name, const vector3d &v) {
	setShaderValue(name, Shader::TypeVector3f, 1, (const float*)v);
}


void ShaderTarget::setShaderValue(const std::string &name, const matrix4x4 &m) {
	setShaderValue(name, Shader::TypeMatrix4x4f, 1, (const float*)m);
}



void ShaderTarget::applyShaderConfigTo(RenderContext* rc) {
	rc->setShader(getShader());

	for(ValueMap::iterator it=values.begin(); it!=values.end(); it++) {
		rc->setShaderValue(it->first, it->second->type, it->second->elements, it->second->data);
	}

	return;
}




ShaderTarget::entry::entry() {
	assert(false);
}

ShaderTarget::entry::entry(Shader::ValueType type, size_t elements) {
	size_t value_size = 0;

	switch(type) {
		case Shader::TypeInt32: {
			value_size = 4;
			break;
		}

		case Shader::TypeFloat: {
			value_size = 4;
			break;
		}

		case Shader::TypeVector2f: {
			value_size = 8;
			break;
		}

		case Shader::TypeVector3f: {
			value_size = 12;
			break;
		}

		case Shader::TypeVector4f: {
			value_size = 16;
			break;
		}

		case Shader::TypeMatrix4x4f: {
			value_size = 64;
			break;
		}
	}

	// ensure, our value type was valid
	assert(value_size > 0);

	this->type      = type;
	this->elements  = elements;
	this->data_size = value_size * elements;
	this->data      = new char[data_size];

	return;
}


ShaderTarget::entry::~entry() {
	if (data) {
		delete data;
	}

	return;
}


bool ShaderTarget::entry::match(Shader::ValueType type, size_t elements) {
	return this->type==type && this->elements == elements;
}


void ShaderTarget::entry::set(const void *pValue) {
	assert(data);
	memcpy(data, pValue, data_size);

	return;
}

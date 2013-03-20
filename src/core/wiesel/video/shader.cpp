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
#include "shader.h"
#include "screen.h"
#include "video_driver.h"

#include "wiesel/util/log.h"
#include <assert.h>
#include <map>

using namespace wiesel;
using namespace wiesel::video;
using namespace std;


const char* Shader::GLSL_VERTEX_SHADER		= "glsl.vert";
const char* Shader::GLSL_FRAGMENT_SHADER	= "glsl.frag";

const char* Shader::HLSL_VERTEX_SHADER		= "hlsl.vert";
const char* Shader::HLSL_FRAGMENT_SHADER	= "hlsl.pixel";



Shader::Shader() {
	return;
}


Shader::~Shader() {
	for(SourcesList::iterator it=sources.begin(); it!=sources.end(); it++) {
		it->second->release();
	}

	return;
}


void Shader::setSource(const std::string &name, DataSource *source) {
	// find old source for this name, if any
	SourcesList::iterator it = sources.find(name);
	if (it != sources.end()) {
		it->second->release();
		sources.erase(it);
	}

	sources[name] = source;
	source->retain();

	return;
}


DataSource *Shader::getSource(const std::string &name) {
	SourcesList::iterator it = sources.find(name);
	if (it != sources.end()) {
		return it->second;
	}

	return NULL;
}


const Shader::SourcesList *Shader::getSources() const {
	return &sources;
}



void Shader::setAttributeName(Shader::Attribute attr, uint8_t index, const std::string &name) {
	if (attributes.size() <= attr) {
		attributes.resize(attr + 1);
	}

	if (attributes[attr].size() <= index) {
		attributes[attr].resize(index + 1);
	}

	attributes[attr][index] = name;

	return;
}


std::string Shader::getAttributeName(Shader::Attribute attr, uint8_t index) const {
	if (attributes.size() > attr && attributes[attr].size() > index) {
		return attributes[attr][index];
	}

	return "";
}


const Shader::AttributeList *Shader::getAttributes() const {
	return &attributes;
}


void Shader::addUniform(const std::string& name) {
	uniform_attributes.insert(name);
}


const std::set<std::string> *Shader::getUniforms() const {
	return &uniform_attributes;
}



bool Shader::doLoadContent() {
	assert(getContent() == NULL);

	// get the video device
	Screen *screen = dynamic_cast<Screen*>(getDevice());
	if (screen == NULL) {
		return false;
	}

	VideoDeviceDriver *driver = screen->getVideoDeviceDriver();
	if (driver == NULL) {
		return false;
	}

	ShaderContent *rc = driver->createShaderContent(this);
	if (rc == NULL) {
		return false;
	}

	setContent(rc);

	return true;
}


bool Shader::doUnloadContent() {
	setContent(NULL);
	return true;
}




ShaderContent::ShaderContent() {
	return;
}

ShaderContent::ShaderContent(Shader *shader) {
	this->shader = shader;
	return;
}

ShaderContent::~ShaderContent() {
	return;
}

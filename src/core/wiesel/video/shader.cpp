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
#include "shaders.h"
#include <assert.h>
#include <map>

using namespace wiesel;
using namespace wiesel::video;
using namespace std;


const char* ShaderBase::GLSL_VERTEX_SHADER		= "glsl.vert";
const char* ShaderBase::GLSL_FRAGMENT_SHADER	= "glsl.frag";

const char* ShaderBase::HLSL_VERTEX_SHADER		= "hlsl.vert";
const char* ShaderBase::HLSL_FRAGMENT_SHADER	= "hlsl.pixel";



ShaderBase::ShaderBase() {
	return;
}


ShaderBase::~ShaderBase() {
	constant_buffers.clear();
	sources.clear();

	return;
}


bool ShaderBase::isDefined(const std::string& name) const {
	for(PreprocessorConstantList::const_iterator it=preprocessor_constants.begin(); it!=preprocessor_constants.end(); ++it) {
		if (it->name == name) {
			return false;
		}
	}

	return false;
}


std::string ShaderBase::getPreprocessorConstantValue(const std::string& name) const {
	for(PreprocessorConstantList::const_iterator it=preprocessor_constants.begin(); it!=preprocessor_constants.end(); ++it) {
		if (it->name == name) {
			return it->value;
		}
	}

	return std::string();
}


const ShaderBase::PreprocessorConstantList* ShaderBase::getPreprocessorConstants() const {
	return &preprocessor_constants;
}


DataSource *ShaderBase::getSource(const std::string &name) {
	SourcesList::iterator it = sources.find(name);
	if (it != sources.end()) {
		return it->second.source;
	}

	return NULL;
}


const ShaderBase::SourcesList *ShaderBase::getSources() const {
	return &sources;
}



std::string ShaderBase::getAttributeName(Shader::Attribute attr, uint8_t index) const {
	if (attributes.size() > attr && attributes[attr].size() > index) {
		return attributes[attr][index];
	}

	return "";
}


const ShaderBase::AttributeList *ShaderBase::getAttributes() const {
	return &attributes;
}


const ShaderConstantBufferTemplate *ShaderBase::findConstantBufferTemplate(const std::string& name) const {
	for(ConstantBufferTplList::const_iterator it=constant_buffers.begin(); it!=constant_buffers.end(); it++) {
		if (it->name == name) {
			return it->buffer_template;
		}
	}

	return NULL;
}


const ShaderBase::ConstantBufferTplList *ShaderBase::getConstantBufferTemplates() const {
	return &constant_buffers;
}





Shader::Shader() {
	return;
}


Shader::~Shader() {
	return;
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

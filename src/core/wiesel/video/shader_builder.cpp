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
#include "shader_builder.h"
#include "shaders.h"

#include "wiesel/io/filesystem.h"

#include <assert.h>
#include <sstream>

using namespace wiesel;
using namespace wiesel::video;



ShaderBuilder::ShaderBuilder() {
	return;
}


ShaderBuilder::~ShaderBuilder() {
}


void ShaderBuilder::setSource(const std::string &name, DataSource *source) {
	// find old source for this name, if any
	SourcesList::iterator it = sources.find(name);
	if (it != sources.end()) {
		sources.erase(it);
	}

	DataSourceEntry entry;
	entry.source = source;

	sources.insert(std::pair<std::string,DataSourceEntry>(name, entry));

	return;
}


bool ShaderBuilder::setSourcesFromFiles(FileSystem* fs, const std::string& filename) {
	int sources_found = 0;
	File *file;

	if ((file = fs->findFile(filename + ".glsl.vert")) != NULL) {
		setSource(GLSL_VERTEX_SHADER, file->asDataSource());
		++sources_found;
	}

	if ((file = fs->findFile(filename + ".glsl.frag")) != NULL) {
		setSource(GLSL_FRAGMENT_SHADER, file->asDataSource());
		++sources_found;
	}

	if ((file = fs->findFile(filename + ".hlsl.vert")) != NULL) {
		setSource(HLSL_VERTEX_SHADER, file->asDataSource());
		++sources_found;
	}

	if ((file = fs->findFile(filename + ".hlsl.frag")) != NULL) {
		setSource(HLSL_FRAGMENT_SHADER, file->asDataSource());
		++sources_found;
	}

	assert(sources_found > 0);

	return sources_found > 0;
}



void ShaderBuilder::setPreprocessorConstant(const std::string& name, const std::string& value) {
	for(PreprocessorConstantList::iterator it=preprocessor_constants.begin(); it!=preprocessor_constants.end(); ++it) {
		if (it->name == name) {
			it->value = value;
			return;
		}
	}

	PreprocessorConstantEntry entry;
	entry.name  = name;
	entry.value = value;

	preprocessor_constants.push_back(entry);

	return;
}


void ShaderBuilder::undefPreprocessorConstant(const std::string& name) {
	for(PreprocessorConstantList::iterator it=preprocessor_constants.begin(); it!=preprocessor_constants.end();) {
		if (it->name == name) {
			it = preprocessor_constants.erase(it);
		}
		else {
			++it;
		}
	}

	return;
}



bool ShaderBuilder::setAttributeName(Shader::Attribute attr, uint8_t index, const std::string &name) {
	if (attributes.size() <= attr) {
		attributes.resize(attr + 1);
	}

	if (attributes[attr].size() <= index) {
		attributes[attr].resize(index + 1);
	}

	attributes[attr][index] = name;

	return true;
}


bool ShaderBuilder::setDefaultAttributeName(Attribute attr, uint8_t index) {
	switch(attr) {
		case VertexPosition: {
			assert(index == 0);

			if (index == 0) {
				return setAttributeName(attr, index, Shaders::ATTRIBUTE_VERTEX_POSITION);
			}

			break;
		}

		case VertexNormal: {
			assert(index == 0);

			if (index == 0) {
				return setAttributeName(attr, index, Shaders::ATTRIBUTE_VERTEX_NORMAL);
			}

			break;
		}

		case VertexColor: {
			assert(index == 0);

			if (index == 0) {
				return setAttributeName(attr, index, Shaders::ATTRIBUTE_VERTEX_COLOR);
			}

			break;
		}

		case VertexTextureCoordinate: {
			std::stringstream attr_name;
			attr_name << Shaders::ATTRIBUTE_VERTEX_TEXTURE_COORDINATE;
			attr_name << static_cast<int>(index);

			return setAttributeName(attr, index, attr_name.str());
		}

		case Texture: {
			std::stringstream attr_name;
			attr_name << Shaders::UNIFORM_TEXTURE;
			attr_name << static_cast<int>(index);

			return setAttributeName(attr, index, attr_name.str());
		}
	}

	return false;
}


bool ShaderBuilder::addConstantBuffer(const std::string& name, uint16_t context, ShaderConstantBufferTemplate* buffer_template) {
	for(ConstantBufferTplList::iterator it=constant_buffers.begin(); it!=constant_buffers.end(); it++) {
		if (it->name == name) {
			// when the existing entry matches the "new" buffer, just add the context (if changed)
			if (it->buffer_template == buffer_template) {
				it->context |= context;
				return true;
			}
			else {
				// otherwise, adding the buffer failed.
				return false;
			}
		}
	}

	// create a new buffer entry
	ConstantBufferTplEntry entry;
	entry.name				= name;
	entry.context			= context;
	entry.buffer_template	= buffer_template;

	// store the buffer
	constant_buffers.push_back(entry);

	// store special buffers
	if (name == Shaders::CONSTANTBUFFER_PROJECTION_MATRIX) {
		constant_buffer_template_projection = buffer_template;
	}

	if (name == Shaders::CONSTANTBUFFER_MODELVIEW_MATRIX) {
		constant_buffer_template_modelview = buffer_template;
	}

	return true;
}


bool ShaderBuilder::addDefaultModelviewMatrixConstantBuffer() {
	return addConstantBuffer(
							Shaders::CONSTANTBUFFER_MODELVIEW_MATRIX,
							Shader::Context_VertexShader,
							Shaders::instance()->getModelviewMatrixBufferTemplate()
	);
}


bool ShaderBuilder::addDefaultProjectionMatrixConstantBuffer() {
	return addConstantBuffer(
							Shaders::CONSTANTBUFFER_PROJECTION_MATRIX,
							Shader::Context_VertexShader,
							Shaders::instance()->getProjectionMatrixBufferTemplate()
	);
}


Shader* ShaderBuilder::create() const {
	Shader *shader = new Shader();

	shader->attributes				= this->attributes;
	shader->sources					= this->sources;
	shader->preprocessor_constants	= this->preprocessor_constants;
	shader->constant_buffers		= this->constant_buffers;

	shader->constant_buffer_template_projection = this->constant_buffer_template_projection;
	shader->constant_buffer_template_modelview  = this->constant_buffer_template_modelview;

	return shader;
}

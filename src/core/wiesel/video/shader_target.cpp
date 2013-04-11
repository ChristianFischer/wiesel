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
#include "shaders.h"

using namespace wiesel;
using namespace wiesel::video;


ShaderTarget::ShaderTarget() {
	this->shader		= NULL;
}

ShaderTarget::~ShaderTarget() {
	for(BufferMap::iterator it=buffers.begin(); it!=buffers.end(); it++) {
		it->second->release();
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

			// release all buffers, which does not belong to the new shader
			for(BufferMap::iterator it=buffers.begin(); it!=buffers.end();) {
				if (this->shader->findConstantBufferTemplate(it->first) == NULL) {
					it->second->release();
					buffers.erase(it++);
				}
				else {
					it++;
				}
			}

			// create buffers, which are new for this shader
			const Shader::ConstantBufferTplList *buffer_templates = this->shader->getConstantBufferTemplates();
			for(Shader::ConstantBufferTplList::const_iterator it=buffer_templates->begin(); it!=buffer_templates->end(); it++) {
				// ignore buffers handled by the renderer itself
				if (
						it->name == Shaders::CONSTANTBUFFER_MODELVIEW_MATRIX
					||	it->name == Shaders::CONSTANTBUFFER_PROJECTION_MATRIX
				) {
					continue;
				}

				// when not already contained in this list, create a new buffer
				if (buffers.find(it->name) == buffers.end()) {
					ShaderConstantBuffer *buffer = new ShaderConstantBuffer(it->buffer_template);
					buffers[it->name] = buffer;
					buffer->retain();
				}
			}
		}
	}

	return;
}


bool ShaderTarget::doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue) {
	bool success = false;

	for(BufferMap::iterator it=buffers.begin(); it!=buffers.end(); it++) {
		success |= it->second->setShaderValue(name, type, elements, pValue);
	}

	return success;
}



void ShaderTarget::applyShaderConfigTo(RenderContext* rc) {
	rc->setShader(getShader());

	for(BufferMap::iterator it=buffers.begin(); it!=buffers.end(); it++) {
		rc->assignShaderConstantBuffer(it->first, it->second);
	}

	return;
}


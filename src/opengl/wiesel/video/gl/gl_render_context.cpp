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
#include "gl_render_context.h"
#include "gl_vertexbuffer_content.h"
#include "gl_indexbuffer_content.h"

#include <wiesel/util/log.h>
#include <wiesel/video/gl/gl.h>
#include <wiesel/video/shader.h>
#include <wiesel/video/texture.h>

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;



OpenGlRenderContext::OpenGlRenderContext(Screen *screen) : RenderContext(screen) {
	this->active_shader				= NULL;
	this->active_shader_content		= NULL;
	return;
}


OpenGlRenderContext::~OpenGlRenderContext() {
	releaseContext();
	return;
}



void OpenGlRenderContext::initContext() {
	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return;
}


void OpenGlRenderContext::releaseContext() {
	setShader(NULL);
	clearTextures();
	return;
}


void OpenGlRenderContext::onSizeChanged(const dimension& size) {
	// setup viewport
	glViewport(0, 0, size.width, size.height);
	return;
}


void OpenGlRenderContext::preRender() {
	// Just fill the screen with a color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CHECK_GL_ERROR;

	return;
}


void OpenGlRenderContext::postRender() {
	// reset all gl objects
	setShader(NULL);
	clearTextures();

	return;
}



void OpenGlRenderContext::setProjectionMatrix(const matrix4x4& matrix) {
	this->projection = matrix;

	if (active_shader_content) {
		active_shader_content->setProjectionMatrix(matrix);
	}

	return;
}


void OpenGlRenderContext::setModelviewMatrix(const matrix4x4& matrix) {
	if (active_shader_content) {
		active_shader_content->setModelviewMatrix(matrix);
	}

	return;
}


void OpenGlRenderContext::setShader(Shader* shader) {
	if (this->active_shader != shader) {
		// clear the old shader
		if (this->active_shader) {
			this->active_shader->release();
			this->active_shader = NULL;
		}

		// ... and it's implementation
		if (this->active_shader_content) {
			this->active_shader_content->release();
			this->active_shader_content = NULL;
		}

		// store the new shader
		if (shader) {
			this->active_shader = shader;
			this->active_shader->retain();

			// load the shader implementation on demand
			if(!active_shader->isLoaded()) {
				active_shader->loadContentFrom(getScreen());
			}

			GlShaderContent *gl_shader_content = dynamic_cast<GlShaderContent*>(active_shader->getContent());
			if (gl_shader_content) {
				this->active_shader_content = gl_shader_content;
				this->active_shader_content->retain();
			}
		}

		// tell OpenGL about the new shader, if any
		if (active_shader_content) {
			glUseProgram(active_shader_content->getGlHandle());

			// update projection matrix for the current shader
			active_shader_content->setProjectionMatrix(getProjectionMatrix());
		}
		else {
			glUseProgram(0);
		}

		CHECK_GL_ERROR;
	}

	return;
}


void OpenGlRenderContext::setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, void *pValue) {
	if (active_shader_content) {
		active_shader_content->setShaderValue(name, type, elements, pValue);
	}

	return;
}



void OpenGlRenderContext::setTexture(uint16_t index, Texture* texture) {
	// check if the texture list is big enough.
	// when this assert fails, there may be missing a prepareTextures call
	assert(index < active_textures.size());

	Texture *active_texture = this->active_textures[index];
	if (active_texture != texture) {
		GlTextureContent *active_texture_content = this->active_textures_content[index];

		// clear the old texture
		if (active_texture) {
			active_texture->release();
			active_texture = NULL;
		}

		// and it's implementation
		if (active_texture_content) {
			active_texture_content->release();
			active_texture_content = NULL;
		}

		// store the new texture
		if (texture) {
			active_texture = texture;
			active_texture->retain();

			GlTextureContent *gl_texture_content = dynamic_cast<GlTextureContent*>(active_texture->getContent());
			if (gl_texture_content) {
				active_texture_content = gl_texture_content;
				active_texture_content->retain();
			}
		}

		// write active textures into the texture list
		active_textures[index]         = active_texture;
		active_textures_content[index] = active_texture_content;
	}

	return;
}


void OpenGlRenderContext::prepareTextureLayers(uint16_t layers) {
	if (active_textures.size() < layers) {
		active_textures.resize(layers, NULL);
		active_textures_content.resize(layers, NULL);
	}
	else {
		// clear all textures above the requested layer number
		for(uint16_t l=layers; l<active_textures.size(); l++) {
			setTexture(l, NULL);
		}
	}

	return;
}

void OpenGlRenderContext::clearTextures() {
	prepareTextureLayers(0);
}



void OpenGlRenderContext::draw(Primitive primitive, const VertexBuffer *vertices) {
	if (bind(vertices)) {
		GLenum mode;

		switch(primitive) {
			case Triangles: {
				mode = GL_TRIANGLES;
				break;
			}

			case TriangleStrip: {
				mode = GL_TRIANGLE_STRIP;
				break;
			}

			case TriangleFan: {
				mode = GL_TRIANGLE_FAN;
				break;
			}
		}

		glDrawArrays(mode, 0, vertices->getSize());
		CHECK_GL_ERROR;

		unbind(vertices);
		CHECK_GL_ERROR;
	}

	return;
}


void OpenGlRenderContext::draw(Primitive primitive, const VertexBuffer *vertices, const IndexBuffer *indices) {
	if (bind(vertices)) {
		GLenum mode;
		GLenum size;

		switch(primitive) {
			case Triangles: {
				mode = GL_TRIANGLES;
				break;
			}

			case TriangleStrip: {
				mode = GL_TRIANGLE_STRIP;
				break;
			}

			case TriangleFan: {
				mode = GL_TRIANGLE_FAN;
				break;
			}
		}

		switch(indices->getBytesPerElement()) {
			case 1: {
				size = GL_UNSIGNED_BYTE;
				break;
			}

			case 2: {
				size = GL_UNSIGNED_SHORT;
				break;
			}

			case 4: {
				size = GL_UNSIGNED_INT;
				break;
			}
		}

		if (bind(indices)) {
			glDrawElements(mode, indices->getSize(), size, NULL);
			unbind(indices);
		}
		else {
			glDrawElements(mode, indices->getSize(), size, indices->getDataPtr());
		}

		CHECK_GL_ERROR;

		unbind(vertices);
		CHECK_GL_ERROR;
	}

	return;
}


bool OpenGlRenderContext::bind(const IndexBuffer* index_buffer) {
	if (index_buffer && index_buffer->getContent()) {
		const GlIndexBufferContent *gl_index_buffer;
		gl_index_buffer = dynamic_cast<const GlIndexBufferContent*>(index_buffer->getContent());

		if (gl_index_buffer) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer->getGlHandle());

			return true;
		}
	}

	return false;
}


void OpenGlRenderContext::unbind(const IndexBuffer *index_buffer) {
	if (index_buffer && index_buffer->getContent()) {
		// just reset the buffer binding
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	return;
}


bool OpenGlRenderContext::bind(const VertexBuffer* vertex_buffer) {
	if (vertex_buffer && active_shader_content) {
		unsigned short				vertex_size		= vertex_buffer->getVertexSize();
		const VertexBuffer::data_t	data			= vertex_buffer->getDataPtr();

		// get the gl vertex buffer
		const GlVertexBufferContent *gl_vertex_buffer;
		gl_vertex_buffer = dynamic_cast<const GlVertexBufferContent*>(vertex_buffer->getContent());

		// data pointer when using no GL buffer, NULL with buffer
		const unsigned char* buffer_offset = NULL;
		if (gl_vertex_buffer && gl_vertex_buffer->getGlHandle()) {
			glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer->getGlHandle());
		}
		else {
			buffer_offset = data;
		}

		// assign vertex positions
		if (vertex_buffer->hasPositions()) {
			GLint  attr_vertex_position = active_shader_content->getAttribHandle(Shader::VertexPosition, 0);
			assert(attr_vertex_position != -1);

			if (attr_vertex_position != -1) {
				glVertexAttribPointer(
							attr_vertex_position,
							vertex_buffer->getPositionDescription().fields,
							GL_FLOAT,
							GL_FALSE,
							vertex_size,
							buffer_offset + vertex_buffer->getPositionDescription().offset
				);

				glEnableVertexAttribArray(attr_vertex_position);
				CHECK_GL_ERROR;
			}
			else {
				return false;
			}
		}

		// assign vertex normals
		if (vertex_buffer->hasNormals()) {
			GLint  attr_vertex_normals = active_shader_content->getAttribHandle(Shader::VertexNormal, 0);
			assert(attr_vertex_normals != -1);

			if (attr_vertex_normals != -1) {
				glVertexAttribPointer(
							attr_vertex_normals,
							vertex_buffer->getNormalDescription().fields,
							GL_FLOAT,
							GL_FALSE,
							vertex_size,
							buffer_offset + vertex_buffer->getNormalDescription().offset
				);

				glEnableVertexAttribArray(attr_vertex_normals);
				CHECK_GL_ERROR;
			}
			else {
				return false;
			}
		}

		// assign vertex colors
		if (vertex_buffer->hasColors()) {
			GLint  attr_vertex_colors = active_shader_content->getAttribHandle(Shader::VertexColor, 0);
			assert(attr_vertex_colors != -1);

			if (attr_vertex_colors != -1) {
				glVertexAttribPointer(
							attr_vertex_colors,
							vertex_buffer->getColorDescription().fields,
							GL_FLOAT,
							GL_FALSE,
							vertex_size,
							buffer_offset + vertex_buffer->getColorDescription().offset
				);

				glEnableVertexAttribArray(attr_vertex_colors);
				CHECK_GL_ERROR;
			}
			else {
				return false;
			}
		}

		// assign textures
		int num_textures = vertex_buffer->getNumberOfTextureLayers();
		for(int i=0; i<num_textures; i++) {
			GLint  attr_vertex_texcoord = active_shader_content->getAttribHandle(Shader::VertexTextureCoordinate, i);
			GLint  attr_vertex_texture  = active_shader_content->getAttribHandle(Shader::Texture, i);
			assert(attr_vertex_texcoord != -1);

			if (attr_vertex_texcoord != -1) {
				glVertexAttribPointer(
						attr_vertex_texcoord,
						vertex_buffer->getTextureDescription(i).fields,
						GL_FLOAT,
						GL_FALSE,
						vertex_size,
						buffer_offset + vertex_buffer->getTextureDescription(i).offset
				);

				glEnableVertexAttribArray(attr_vertex_texcoord);
				CHECK_GL_ERROR;

				GlTextureContent *texture_content = active_textures_content[i];
				if (texture_content && attr_vertex_texture != -1) {
					assert(attr_vertex_texture  != -1);

					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, texture_content->getGlHandle());
					glUniform1i(attr_vertex_texture, i);
					CHECK_GL_ERROR;
				}
			}
			else {
				return false;
			}
		}

		return true;
	}

	return false;
}


void OpenGlRenderContext::unbind(const VertexBuffer* vertex_buffer) {
	if (vertex_buffer && active_shader_content) {
		if (vertex_buffer->hasPositions()) {
			GLint attr_vertex_position = active_shader_content->getAttribHandle(Shader::VertexPosition, 0);
			glDisableVertexAttribArray(attr_vertex_position);
		}

		if (vertex_buffer->hasNormals()) {
			GLint attr_vertex_normals =active_shader_content->getAttribHandle(Shader::VertexNormal, 0);
			glDisableVertexAttribArray(attr_vertex_normals);
		}

		if (vertex_buffer->hasColors()) {
			GLint attr_vertex_colors = active_shader_content->getAttribHandle(Shader::VertexColor, 0);
			glDisableVertexAttribArray(attr_vertex_colors);
		}

		int num_textures = vertex_buffer->getNumberOfTextureLayers();
		for(int i=0; i<num_textures; i++) {
			GLint attr_vertex_texcoord = active_shader_content->getAttribHandle(Shader::VertexTextureCoordinate, 0);
			glDisableVertexAttribArray(attr_vertex_texcoord);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return;
}


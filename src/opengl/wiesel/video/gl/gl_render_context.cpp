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
#include "gl_video_driver.h"
#include "wiesel/video/shaders.h"
#include "wiesel/video/video_driver.h"

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
}


void OpenGlRenderContext::setModelviewMatrix(const matrix4x4& matrix) {
	if (active_shader && active_shader_content) {

		// get the active shader's matrix buffer template
		ShaderConstantBufferTemplate *modelview_buffer_template = NULL;
		modelview_buffer_template = active_shader->getModelviewMatrixConstantBufferTemplate();

		if (modelview_buffer_template) {
			// get the template's shared buffer
			ShaderConstantBuffer *modelview_buffer = modelview_buffer_template->getSharedBuffer();

			// update the modelview value
			bool was_set = modelview_buffer->setShaderValueAt(0, matrix);
			assert(was_set);

			// get the buffer's content
			ShaderConstantBufferContent *modelview_buffer_content = modelview_buffer->getContent();
			if (modelview_buffer_content == NULL) {
				modelview_buffer->loadContentFrom(getScreen());
				modelview_buffer_content = modelview_buffer->getContent();
				assert(modelview_buffer_content);
			}

			active_shader_content->assignShaderConstantBuffer(
										modelview_buffer_template,
										modelview_buffer_content
			);
		}
	}

	return;
}


void OpenGlRenderContext::setShader(Shader* shader) {
	if (this->active_shader != shader) {
		// clear the old shader
		safe_release(this->active_shader);

		// ... and it's implementation
		safe_release(this->active_shader_content);

		// store the new shader
		if (shader) {
			this->active_shader = keep(shader);

			// load the shader implementation on demand
			if(!active_shader->isLoaded()) {
				active_shader->loadContentFrom(getScreen());
			}

			GlShaderContent *gl_shader_content = dynamic_cast<GlShaderContent*>(active_shader->getContent());
			if (gl_shader_content) {
				this->active_shader_content = keep(gl_shader_content);
			}
		}

		// tell OpenGL about the new shader, if any
		if (active_shader_content) {
			glUseProgram(active_shader_content->getGlHandle());

			// get the shader's projection matrix buffer template
			ShaderConstantBufferTemplate *projection_buffer_template;
			projection_buffer_template = active_shader->getProjectionMatrixConstantBufferTemplate();

			if (projection_buffer_template) {
				// get the template's shared buffer
				ShaderConstantBuffer *projection_buffer = projection_buffer_template->getSharedBuffer();

				// get the data pointer
				ShaderConstantBuffer::data_t projection_data_ptr = projection_buffer->getShaderDataPointerAt(0);

				// check if the projection matrix has changed
				if (this->projection != *(reinterpret_cast<const matrix4x4*>(projection_data_ptr))) {
					projection_buffer->setShaderValueAt(0, this->projection);
				}

				// get the buffer's content
				ShaderConstantBufferContent *projection_buffer_content = projection_buffer->getContent();
				if (projection_buffer_content == NULL) {
					projection_buffer->loadContentFrom(getScreen());
					projection_buffer_content = projection_buffer->getContent();
					assert(projection_buffer_content);
				}

				// update projection matrix for the current shader
				active_shader_content->assignShaderConstantBuffer(
											projection_buffer_template,
											projection_buffer_content
				);
			}
		}
		else {
			glUseProgram(0);
		}

		CHECK_GL_ERROR;
	}

	return;
}


bool OpenGlRenderContext::assignShaderConstantBuffer(const ShaderConstantBufferTemplate *buffer_template, ShaderConstantBuffer *buffer) {
	if (active_shader_content) {
		// load on demand
		if (buffer->isLoaded() == false) {
			buffer->loadContentFrom(getScreen());

			// failed?
			if (buffer->isLoaded() == false) {
				return false;
			}
		}

		return active_shader_content->assignShaderConstantBuffer(buffer_template, buffer->getContent());
	}

	return false;
}



void OpenGlRenderContext::setTexture(uint16_t index, Texture* texture) {
	// check if the texture list is big enough.
	// when this assert fails, there may be missing a prepareTextures call
	assert(index < active_textures.size());

	Texture *active_texture = this->active_textures[index];
	if (active_texture != texture) {
		GlTextureContent *active_texture_content = this->active_textures_content[index];

		// clear the old texture
		safe_release(active_texture);

		// and it's implementation
		safe_release(active_texture_content);

		// store the new texture
		if (texture) {
			active_texture = keep(texture);

			active_texture_content = dynamic_cast<GlTextureContent*>(active_texture->getContent());
		}

		if (active_texture_content) {
			keep(active_texture_content);

			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, active_texture_content->getGlHandle());
		}
		else {
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, 0);
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
		}

		// assign vertex normals
		if (vertex_buffer->hasNormals()) {
			GLint  attr_vertex_normals = active_shader_content->getAttribHandle(Shader::VertexNormal, 0);

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
		}

		// assign vertex colors
		if (vertex_buffer->hasColors()) {
			GLint  attr_vertex_colors = active_shader_content->getAttribHandle(Shader::VertexColor, 0);

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
		}

		// assign textures
		int num_textures = vertex_buffer->getNumberOfTextureLayers();
		for(int i=0; i<num_textures; i++) {
			GLint  attr_vertex_texcoord = active_shader_content->getAttribHandle(Shader::VertexTextureCoordinate, i);
			GLint  attr_vertex_texture  = active_shader_content->getAttribHandle(Shader::Texture, i);

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
			}

			if (attr_vertex_texture != -1) {
				GlTextureContent *texture_content = active_textures_content[i];

				if (texture_content) {
				//	glActiveTexture(GL_TEXTURE0 + i);
				//	glBindTexture(GL_TEXTURE_2D, texture_content->getGlHandle());
					glUniform1i(attr_vertex_texture, i);
					CHECK_GL_ERROR;
				}
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
			GLint attr_vertex_normals = active_shader_content->getAttribHandle(Shader::VertexNormal, 0);
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


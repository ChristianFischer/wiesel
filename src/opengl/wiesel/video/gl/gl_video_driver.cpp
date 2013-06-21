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
#include "gl_video_driver.h"

#include "gl_indexbuffer_content.h"
#include "gl_vertexbuffer_content.h"
#include "gl_shader_content.h"
#include "gl_shader_constantbuffer_content.h"
#include "gl_texture_content.h"
#include "gl_render_buffer_content.h"

#include <wiesel/util/log.h>
#include <wiesel/ui/touchhandler.h>
#include <assert.h>

#include <algorithm>


using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;



OpenGlVideoDeviceDriver::OpenGlVideoDeviceDriver(Screen *screen) : VideoDeviceDriver(screen) {
	return;
}


OpenGlVideoDeviceDriver::~OpenGlVideoDeviceDriver() {
	return;
}


bool OpenGlVideoDeviceDriver::initOpenGl() {
	// fetch OpenGL information
	info.api					= "OpenGL";
	info.api_version			= (const char*)(glGetString(GL_VERSION));
	info.api_vendor				= (const char*)(glGetString(GL_VENDOR));
	info.renderer				= (const char*)(glGetString(GL_RENDERER));

	info.shaders.api			= "GLSL";
	info.shaders.api_version	= (const char*)(glGetString(GL_SHADING_LANGUAGE_VERSION));

	glGetIntegerv(GL_MAX_TEXTURE_SIZE,	(GLint*)&info.textures.max_size);

	#if defined(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,	(GLint*)&info.textures.max_texture_units);
	#elif defined(GL_MAX_TEXTURE_UNITS)
		glGetIntegerv(GL_MAX_TEXTURE_UNITS,	(GLint*)&info.textures.max_texture_units);
	#else
		#error Impossible to determine maximum number of texture units
	#endif

	std::string extensions		= (const char*)(glGetString(GL_EXTENSIONS));
	info.extensions.push_back(extensions);

	// try to split the string into seperate items
	std::string::size_type p1 = 0;
	std::string::size_type p2 = 0;
	while((p2 = extensions.find(" ", p1)) != std::string::npos) {
		std::string extension = extensions.substr(p1, (p2 - p1));
		info.extensions.push_back(extension);
		p1 = p2 + 1;
	}

	// add the last extension
	std::string extension = extensions.substr(p1);
	info.extensions.push_back(extension);

	// check some specific extensions
	if (std::find(info.extensions.begin(), info.extensions.end(), "GL_OES_texture_npot") != info.extensions.end()) {
		// the renderer suppots non-pot textures
		info.textures.requires_pot = false;
	}

	CHECK_GL_ERROR;

	return true;
}


vector2d OpenGlVideoDeviceDriver::convertScreenToWorld(const vector2d &screen) const {
	// at first, convert into OpenGL coordinate space
	vector2d transformed = vector2d(
			+(screen.x / getResolution().width  - 0.5f) * 2,
			-(screen.y / getResolution().height - 0.5f) * 2
	);

	// then use the projection- and modelview matrix to get the world coordinate system
	return
			transformed
		/	getProjectionMatrix()
	;

}


void OpenGlVideoDeviceDriver::preRender() {
	return;
}


void OpenGlVideoDeviceDriver::postRender() {
	return;
}



IndexBufferContent *OpenGlVideoDeviceDriver::createIndexBufferContent(IndexBuffer *index_buffer) {
	return GlIndexBufferContent::createContentFor(index_buffer);
}

VertexBufferContent *OpenGlVideoDeviceDriver::createVertexBufferContent(VertexBuffer *vertex_buffer) {
	return GlVertexBufferContent::createContentFor(vertex_buffer);
}

ShaderContent *OpenGlVideoDeviceDriver::createShaderContent(Shader *shader) {
	return GlShaderContent::createContentFor(shader);
}

ShaderConstantBufferContent *OpenGlVideoDeviceDriver::createConstantBufferContent(ShaderConstantBuffer *shader_constant_buffer) {
	return GlShaderConstantBufferContent::createContentFor(shader_constant_buffer);
}

TextureContent *OpenGlVideoDeviceDriver::createTextureContent(Texture *texture) {
	return GlTextureContent::createContentFor(texture);
}

RenderBufferContent *OpenGlVideoDeviceDriver::createRenderBufferContent(RenderBuffer *render_buffer) {
	return GlRenderBufferContent::createContentFor(render_buffer);
}


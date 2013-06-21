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
#include "gl_render_buffer_content.h"
#include "gl_texture_content.h"
#include "wiesel/util/log.h"
#include "wiesel/video/render_context.h"
#include "wiesel/video/video_driver.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;


GlRenderBufferContent::GlRenderBufferContent(RenderBuffer *render_buffer) : RenderBufferContent(render_buffer) {
	gl_render_buffer = 0;
	gl_frame_buffer  = 0;

	return;

}


GlRenderBufferContent::~GlRenderBufferContent() {
	releaseBuffers();
}


GlRenderBufferContent* GlRenderBufferContent::createContentFor(RenderBuffer* render_buffer) {
	GlRenderBufferContent *content = new GlRenderBufferContent(render_buffer);
	if (content->initializeBuffers() == false) {
		delete content;
		return NULL;
	}

	return content;
}


bool GlRenderBufferContent::initializeBuffers() {
	bool success = true;

	/*
	glGenRenderbuffers(1, &gl_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gl_render_buffer);
	CHECK_GL_ERROR;

	glRenderbufferStorage(
						GL_RENDERBUFFER,
						GL_DEPTH_COMPONENT24,
						getRenderBuffer()->getSize().width,
						getRenderBuffer()->getSize().height
	);
	CHECK_GL_ERROR;
	*/

	glGenFramebuffers(1, &gl_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gl_frame_buffer);
	CHECK_GL_ERROR;

	const RenderBuffer::TextureList *textures = getRenderBuffer()->getTargetTextures();
	int texture_index = 0;
	gl_draw_buffers.clear();

	for(RenderBuffer::TextureList::const_iterator it=textures->begin(); it!=textures->end(); ++it) {
		GlTextureContent *gl_texture_content = dynamic_cast<GlTextureContent*>(it->texture->getContent());
		GLuint texture_handle = 0;

		if (gl_texture_content) {
			texture_handle = gl_texture_content->getGlHandle();
		}

		glFramebufferTexture2D(
							GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0 + texture_index,
							GL_TEXTURE_2D,
							texture_handle,
							0
		);

		gl_draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + texture_index);

		++texture_index;
		CHECK_GL_ERROR;
	}

	/*
	glFramebufferRenderbuffer(
						GL_FRAMEBUFFER,
						GL_DEPTH_ATTACHMENT,
						GL_RENDERBUFFER,
						gl_render_buffer
	);
	CHECK_GL_ERROR;
	*/

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::err << "Framebuffer not supported" << std::endl;
		success = false;
		assert(false);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return success;
}


void GlRenderBufferContent::releaseBuffers() {
	if (gl_render_buffer) {
		glDeleteRenderbuffers(1, &gl_render_buffer);
		gl_render_buffer = 0;
	}

	if (gl_frame_buffer) {
		glDeleteFramebuffers(1, &gl_frame_buffer);
		gl_frame_buffer = 0;
	}

	return;
}


bool GlRenderBufferContent::enableRenderBuffer(RenderContext* render_context) {
	glBindRenderbuffer(GL_RENDERBUFFER, gl_render_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,   gl_frame_buffer);

//	glDrawBuffers(gl_draw_buffers.size(), gl_draw_buffers.data());

	glViewport(
			getRenderBuffer()->getViewport().position.x,
			getRenderBuffer()->getViewport().position.y,
			getRenderBuffer()->getViewport().size.width,
			getRenderBuffer()->getViewport().size.height
	);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}

	return true;
}


void GlRenderBufferContent::disableRenderBuffer(RenderContext *render_context) {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	dimension screen_size = render_context
			->getScreen()
			->getVideoDeviceDriver()
			->getResolution()
	;

	glViewport(
			0,
			0,
			screen_size.width,
			screen_size.height
	);

	return;
}


void GlRenderBufferContent::preRender(RenderContext *render_context) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return;
}


void GlRenderBufferContent::postRender(RenderContext *render_context) {
	return;
}


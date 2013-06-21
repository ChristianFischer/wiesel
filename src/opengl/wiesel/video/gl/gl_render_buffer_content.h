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
#ifndef __WIESEL_VIDEO_GL_RENDERTARGET_CONTENT_H__
#define __WIESEL_VIDEO_GL_RENDERTARGET_CONTENT_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/video/render_buffer.h>

#include "gl.h"


namespace wiesel {
namespace video {
namespace gl {

	class WIESEL_OPENGL_EXPORT GlRenderBufferContent : public RenderBufferContent
	{
	private:
		GlRenderBufferContent(RenderBuffer *render_buffer);

	public:
		virtual ~GlRenderBufferContent();

		static GlRenderBufferContent* createContentFor(RenderBuffer *render_buffer);

	private:
		bool initializeBuffers();
		void releaseBuffers();

	public:
		virtual bool enableRenderBuffer(RenderContext *render_context);
		virtual void disableRenderBuffer(RenderContext *render_context);
		virtual void preRender(RenderContext *render_context);
		virtual void postRender(RenderContext *render_context);

	public:
		/**
		 * @brief Get the OpenGL handle of the according framebuffer.
		 */
		inline GLuint getGlFrameBufferHandle() const {
			return gl_frame_buffer;
		}

	private:
		std::vector<GLenum>		gl_draw_buffers;
		GLuint					gl_render_buffer;
		GLuint					gl_frame_buffer;
	};

} // namespace gl
} // namespace video
} // namespace wiesel

#endif // __WIESEL_VIDEO_GL_RENDERTARGET_CONTENT_H__

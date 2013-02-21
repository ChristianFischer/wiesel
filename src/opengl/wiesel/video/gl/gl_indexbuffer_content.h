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
#ifndef __WIESEL_VIDEO_GL_INDEXBUFFER_CONTENT_H__
#define __WIESEL_VIDEO_GL_INDEXBUFFER_CONTENT_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/video/indexbuffer.h>

#include "gl.h"


namespace wiesel {
namespace video {
namespace gl {


	/**
	 * @brief OpenGL backend for indexbuffer objects.
	 */
	class WIESEL_OPENGL_EXPORT GlIndexBufferContent : public IndexBufferContent
	{
	private:
		GlIndexBufferContent(IndexBuffer *ib);

	public:
		virtual ~GlIndexBufferContent();

		static GlIndexBufferContent *createContentFor(IndexBuffer *ib);

	public:
		/**
		 * @brief Initialize the index buffer data on the graphics hardware.
		 */
		void initializeIndexBuffer();

		/**
		 * @brief Releases the index buffer object on the graphics hardware.
		 */
		void releaseIndexBuffer();

		/**
		 * @brief Get the OpenGL handle of the assigned buffer.
		 */
		inline GLuint getGlHandle() const {
			return handle;
		}

	private:
		GLuint		handle;
	};

} /* namespace gl */
} /* namespace video */
} /* namespace wiesel */
#endif /* __WIESEL_VIDEO_GL_INDEXBUFFER_CONTENT_H__ */

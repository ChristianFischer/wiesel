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
#ifndef __WIESEL_VIDEO_GL_TEXTURE_CONTENT_H__
#define __WIESEL_VIDEO_GL_TEXTURE_CONTENT_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/geometry.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/resources/graphics/image.h>
#include <wiesel/video/texture.h>
#include <string>

#include "gl.h"


namespace wiesel {
namespace video {
namespace gl {

	/**
	 * @brief OpenGL Backend for texture objects.
	 * @return
	 */
	class WIESEL_OPENGL_EXPORT GlTextureContent : public TextureContent
	{
	private:
		GlTextureContent(Texture *texture);

	public:
		virtual ~GlTextureContent();

		/**
		 * @brief Crerates an OpenGL texture content for the given texture.
		 * @param texture	The texture object where to load the content object from.
		 * @return A content object on success, \c NULL when failed.
		 */
		static WIESEL_OPENGL_EXPORT GlTextureContent *createContentFor(Texture *texture);

		/**
		 * @brief get the OpenGL texture handle.
		 */
		inline GLuint getGlHandle() const {
			return handle;
		}

	private:
		bool initTexture();

		bool loadEmptyTexture(PixelFormat format, const dimension& size);
		bool loadTextureFromSource(DataSource *data);

		/// creates the texture on hardware
		bool createHardwareTexture(PixelFormat format, const dimension& size, DataBuffer* data);

		/// release the texture.
		void releaseTexture();

	private:
		GLuint			handle;
	};

} /* namespace gl */
} /* namespace video */
} /* namespace wiesel */
#endif /* __WIESEL_VIDEO_GL_TEXTURE_CONTENT_H__ */

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
#ifndef __WIESEL_GL_TEXTURE_TEXTURE_H__
#define __WIESEL_GL_TEXTURE_TEXTURE_H__

#include <wiesel/gl/gl.h>
#include <wiesel/geometry.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <string>


namespace wiesel {

	class Texture : public virtual SharedObject
	{
	private:
		Texture();

	public:
		virtual ~Texture();

		/**
		 * @brief load a texture from a \ref File.
		 */
		static Texture *fromFile(File *filename);

		/**
		 * @brief load a texture from a \ref DataSource.
		 */
		static Texture *fromDataSource(DataSource *data);

		/**
		 * @brief get the texture's size.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getSize() const {
			return size;
		}

		/**
		 * @brief get the texture's original size, as it was stored in the texture image.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getOriginalSize() const {
			return original_size;
		}

		/**
		 * @brief get the OpenGL texture handle.
		 */
		inline GLuint getGlHandle() const {
			return handle;
		}

	private:
		/// creates the texture on hardware
		bool createHardwareTexture();

		/// release the texture.
		void release_texture();

	private:
		DataSource*		data;
		dimension		size;
		dimension		original_size;
		GLuint			handle;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_TEXTURE_H__ */

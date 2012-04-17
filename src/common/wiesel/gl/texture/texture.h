/*
 * texture.h
 *
 *  Created on: 07.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_GL_TEXTURE_TEXTURE_H__
#define __WIESEL_GL_TEXTURE_TEXTURE_H__

#include <string>
#include <wiesel/gl/gl.h>
#include <wiesel/geometry.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>


namespace wiesel {

	class Texture : public SharedObject
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

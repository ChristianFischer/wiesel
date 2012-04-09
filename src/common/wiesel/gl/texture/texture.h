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


namespace wiesel {

	class Texture
	{
	private:
		Texture();

	public:
		virtual ~Texture();

		/**
		 * @brief load a texture from a file.
		 */
		static Texture *fromFile(const std::string &filename);

		/**
		 * @brief release the texture.
		 * Needs to be used before deleting this object.
		 */
		void release();

		/**
		 * @brief get the OpenGL texture handle.
		 */
		inline GLuint getGlHandle() const {
			return handle;
		}

	private:
		/// creates the texture on hardware
		bool createHardwareTexture();

	private:
		std::string		filename;
		dimension		size;
		GLuint			handle;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_TEXTURE_H__ */

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


namespace wiesel {

	class Texture : public SharedObject
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
		std::string		filename;
		dimension		size;
		GLuint			handle;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_TEXTURE_H__ */

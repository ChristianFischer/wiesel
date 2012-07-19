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
#ifndef __WIESEL_GL_TEXTURE_SPRITEFRAME_H__
#define __WIESEL_GL_TEXTURE_SPRITEFRAME_H__

#include <wiesel/gl/gl.h>
#include <wiesel/gl/texture/texture.h>
#include <wiesel/geometry.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/io/datasource.h>

#include <ostream>
#include <string>


namespace wiesel {


	/**
	 * @brief An object which contains a specific part of a texture.
	 */
	class SpriteFrame : public virtual SharedObject
	{
	private:
		SpriteFrame();

	public:
		/**
		 * @brief Creates a new SpriteFrame, which covers the full area of a texture.
		 * @param name			The name of the new SpriteFrame.
		 * @param texture		The SpriteFrame's texture.
		 */
		SpriteFrame(const std::string &name, Texture *texture);

		/**
		 * @brief Creates a new SpriteFrame.
		 * @param name			The name of the new SpriteFrame.
		 * @param texture		The SpriteFrame's texture.
		 * @param texture_rect	The area within this recture, which belongs to this SpriteFrame.
		 */
		SpriteFrame(const std::string &name, Texture *texture, const rect &texture_rect);

		virtual ~SpriteFrame();

	public:
		/// get the SpriteFrame's name.
		inline const std::string& getName() const {
			return name;
		}

		/// get the texture, which contains the SpriteFrame's image data.
		inline const Texture *getTexture() const {
			return texture;
		}

		/// get the texture, which contains the SpriteFrame's image data.
		inline Texture *getTexture() {
			return texture;
		}

		/// get the area within the texture, which belongs to this SpriteFrame.
		inline const rect& getTextureRect() const {
			return texture_rect;
		}

	private:
		std::string		name;
		Texture*		texture;
		rect			texture_rect;
	};



	std::ostream& operator <<(std::ostream&, const SpriteFrame *sprite_frame);


} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_SPRITEFRAME_H__ */

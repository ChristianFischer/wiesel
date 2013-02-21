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

#include <wiesel/wiesel-core.def>

#include <wiesel/io/datasource.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/video/texture.h>
#include <wiesel/geometry.h>

#include <ostream>
#include <string>


namespace wiesel {


	/**
	 * @brief An object which contains a specific part of a texture.
	 */
	class WIESEL_CORE_EXPORT SpriteFrame : public virtual SharedObject
	{
	private:
		SpriteFrame();

	public:
		struct TextureCoords {
			vector2d	tl;
			vector2d	tr;
			vector2d	bl;
			vector2d	br;
		};

	public:
		/**
		 * @brief Creates a new SpriteFrame, which covers the full area of a texture.
		 * @param name			The name of the new SpriteFrame.
		 * @param texture		The SpriteFrame's texture.
		 */
		SpriteFrame(const std::string &name, video::Texture *texture);

		/**
		 * @brief Creates a new SpriteFrame.
		 * @param name			The name of the new SpriteFrame.
		 * @param texture		The SpriteFrame's texture.
		 * @param texture_rect	The area within this texture, which belongs to this SpriteFrame.
		 */
		SpriteFrame(const std::string &name, video::Texture *texture, const rectangle &texture_rect);

		/**
		 * @brief Creates a new SpriteFrame.
		 * @param name			The name of the new SpriteFrame.
		 * @param texture		The SpriteFrame's texture.
		 * @param size			The SpriteFrame's total size, including any trimmed transparency area.
		 * @param inner_rect	The area within the sprite, which contains the texture image.
		 * @param texcoords		The texture coordinates of the sprite's vertices.
		 */
		SpriteFrame(
				const std::string &name,
				video::Texture *texture,
				const dimension &size,
				const rectangle &inner_rect,
				const TextureCoords &texcoords
		);

		virtual ~SpriteFrame();

	public:
		/// get the SpriteFrame's name.
		inline const std::string& getName() const {
			return name;
		}

		/// get the texture, which contains the SpriteFrame's image data.
		inline const video::Texture *getTexture() const {
			return texture;
		}

		/// get the texture, which contains the SpriteFrame's image data.
		inline video::Texture *getTexture() {
			return texture;
		}

		/// get this frame's total size
		inline const dimension& getSize() const {
			return sprite_size;
		}

		/// get the area within the texture, which belongs to this SpriteFrame.
		inline const rectangle& getInnerRect() const {
			return inner_rect;
		}

		/// get the texture coordinates of this sprite
		inline const TextureCoords& getTextureCoordinates() const {
			return texture_coordinates;
		}

	private:
		std::string		name;
		video::Texture*	texture;
		dimension		sprite_size;
		rectangle		inner_rect;
		TextureCoords	texture_coordinates;
	};



	WIESEL_CORE_EXPORT std::ostream& operator <<(std::ostream&, const SpriteFrame *sprite_frame);


} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_SPRITEFRAME_H__ */

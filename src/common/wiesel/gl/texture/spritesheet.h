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
#ifndef __WIESEL_GL_TEXTURE_SPRITESHEET_H__
#define __WIESEL_GL_TEXTURE_SPRITESHEET_H__

#include "spriteframe.h"

#include <wiesel/gl/gl.h>
#include <wiesel/gl/texture/texture.h>
#include <wiesel/geometry.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>

#include <string>
#include <map>
#include <vector>


namespace wiesel {

	// predeclarations

	class File;


	/**
	 * @brief A SpriteSheet contains multiple sprites which belongs to a single texture.
	 */
	class SpriteSheet : public virtual SharedObject
	{
	public:
		typedef std::vector<SpriteFrame*>			List;
		typedef std::map<std::string,SpriteFrame*>	NamedList;

	private:
		SpriteSheet();

	public:
		/**
		 * @brief Create a new, empty spritesheet, based on an existing texture.
		 */
		SpriteSheet(Texture *texture);

		/**
		 * @brief Load a spritesheet from a file.
		 * @param file	A file containing the spritesheet.
		 */
		static SpriteSheet* fromFile(File *file);

		virtual ~SpriteSheet();

	public:
		/**
		 * @brief Adds a new SpriteFrame to this spritesheet.
		 */
		void add(SpriteFrame *sprite);

		/**
		 * @brief Creates an alias name for an existing sprite.
		 * @param	alias		The new alias name for this sprite.
		 * @param	original	The name of the original sprite.
		 */
		void addAlias(const std::string &alias, const std::string &original);

		/**
		 * @brief Creates an alias name for an existing sprite.
		 * @param	alias		The new alias name for this sprite.
		 * @param	index		The index of the original sprite.
		 */
		void addAlias(const std::string &alias, size_t index);

		/**
		 * @brief Get a sprite by it's name.
		 */
		SpriteFrame *get(const std::string &name);

		/**
		 * @brief Get the list of all sprites in this spritesheet.
		 */
		inline const List *getSprites() const {
			return &sprite_list;
		}

	public:
		/// get this SpriteFrame's texture.
		inline Texture *getTexture() {
			return texture;
		}

	private:
		Texture*		texture;

		List			sprite_list;
		NamedList		sprites_by_name;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_SPRITESHEET_H__ */

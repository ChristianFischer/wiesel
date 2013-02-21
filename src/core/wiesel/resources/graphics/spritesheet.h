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

#include <wiesel/wiesel-core.def>

#include "spriteframe.h"

#include <wiesel/io/datasource.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/video/texture.h>
#include <wiesel/geometry.h>

#include <string>
#include <map>
#include <vector>


namespace wiesel {

	// predeclarations

	class File;


	/**
	 * @brief A SpriteSheet contains multiple sprites which belongs to a single texture.
	 */
	class WIESEL_CORE_EXPORT SpriteSheet : public virtual SharedObject
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
		SpriteSheet(video::Texture *texture);

		/**
		 * @brief Load a spritesheet from a file.
		 * @param file	A file containing the spritesheet.
		 */
		static WIESEL_CORE_EXPORT SpriteSheet* fromFile(File *file);

		virtual ~SpriteSheet();

	public:
		/**
		 * @brief Adds a new SpriteFrame to this spritesheet.
		 */
		void add(SpriteFrame *sprite);

		/**
		 * @brief Creates multiple frames of an rasterized spritesheet.
		 * @param frames_x		Number of frames in X direction in this spritesheet.
		 * @param frames_y		Number of frames in Y direction in this spritesheet.
		 * @param start_at		Start creating frames at the given index.
		 * @param num_frames	Determine the number of frames to be created.
		 * @param start_with	Start with this character as frame name.
		 *						Each following frame gets an incremented value of this name.
		 */
		void createRasterFrames(
								unsigned int frames_x,
								unsigned int frames_y,
								unsigned int start_at,
								unsigned int num_frames,
								char start_with
		);

		/**
		 * @brief Creates multiple frames of an rasterized spritesheet.
		 * @param frames_x		Number of frames in X direction in this spritesheet.
		 * @param frames_y		Number of frames in Y direction in this spritesheet.
		 * @param start_at		Start creating frames at the given index.
		 * @param mapping		A null-terminated string where each character
		 *						will be used as the name of a new frame.
		 */
		void createRasterFrames(
								unsigned int frames_x,
								unsigned int frames_y,
								unsigned int start_at,
								const char *mapping
		);

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
		 * @brief Removes all sprites from this spritesheet.
		 */
		void clear();

		/**
		 * @brief Get the list of all sprites in this spritesheet.
		 */
		inline const List *getSprites() const {
			return &sprite_list;
		}

	public:
		/// get this SpriteFrame's texture.
		inline video::Texture *getTexture() {
			return texture;
		}

	private:
		video::Texture*	texture;

		List			sprite_list;
		NamedList		sprites_by_name;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_SPRITESHEET_H__ */

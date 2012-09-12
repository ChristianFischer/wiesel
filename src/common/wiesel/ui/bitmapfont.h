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
#ifndef __WIESEL_UI_BITMAPFONT_H__
#define __WIESEL_UI_BITMAPFONT_H__

#include <wiesel/wiesel-common.def>

#include "font.h"

namespace wiesel {

	// predeclaration

	class SpriteFrame;
	class SpriteSheet;
	class ShaderProgram;



	/**
	 * @brief A font class implementing a bitmap font, provided from a spritesheet.
	 */
	class WIESEL_COMMON_EXPORT BitmapFont : public Font
	{
	public:
		/**
		 * @brief Creates a new, empty BitmapFont object.
		 */
		BitmapFont();

		/**
		 * @brief Creates a new BitmapFont object with a SpriteSheet as source.
		 */
		BitmapFont(SpriteSheet *ss);

		virtual ~BitmapFont();
		
	// setter / getter
	public:
		/**
		 * @brief Set the SpriteSheet, which provides the images for this font.
		 */
		void setSpriteSheet(SpriteSheet *ss);

		/**
		 * @brief Get the SpriteSheet assigned to this node.
		 */
		inline SpriteSheet *getSpriteSheet() const {
			return spritesheet;
		}

		/**
		 * @brief Set the preferred shader to be used for rendering.
		 * Will be set for the target label node, if there is not already a shader configured.
		 */
		void setShader(ShaderProgram *shader);

		/**
		 * @brief Get the preferred shader for this font.
		 */
		inline ShaderProgram *getShader() const {
			return shader;
		}

		/**
		 * @brief Sets an additional distance between characters.
		 * This value can be negative to make characters overlap.
		 */
		void setCharSpacing(float w);

		/**
		 * @brief Get the current spacing between characters.
		 */
		inline float getCharSpacing() const {
			return char_spacing;
		}

		/**
		 * @brief Sets an additional distance between lines.
		 */
		void setLineSpacing(float w);

		/**
		 * @brief Get the current spacing between lines.
		 */
		inline float getLineSpacing() const {
			return line_spacing;
		}

	// implement Font members
	public:
		virtual void onAssignToNode(MultiSpriteNode *node) const;
		virtual void onDeassignFromNode(MultiSpriteNode *node) const;

		virtual float getCharWidth(char c) const;
		virtual float getCharHeight(char c) const;

		virtual void fillLabelNode(
						MultiSpriteNode *msn,
						const std::vector<std::string> &text,
						float align=0.0f
		) const;

	protected:
		/**
		 * @brief Find a SpriteFrame within the current SpriteSheet,
		 * which represents the given character.
		 * @return The matching SpriteFrame, or NULL, if no SpriteFrame was found.
		 */
		virtual SpriteFrame *findSpriteForChar(char c) const;

	private:
		SpriteSheet*		spritesheet;
		ShaderProgram*		shader;

		float				char_spacing;
		float				line_spacing;
	};

}

#endif	// __WIESEL_UI_BITMAPFONT_H__

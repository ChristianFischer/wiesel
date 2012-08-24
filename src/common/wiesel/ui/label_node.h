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
#ifndef __WIESEL_UI_LABELNODE_H__
#define	__WIESEL_UI_LABELNODE_H__

#include "wiesel/graph/2d/multisprite_node.h"

#include <string>


namespace wiesel {

	// predeclaration
	class Font;


	/**
	 * @brief A node which can display multiline texts.
	 */
	class LabelNode : public MultiSpriteNode
	{
	public:
		LabelNode();
		virtual ~LabelNode();

		/**
		 * @brief Set the current font which will be used to display text for this label.
		 */
		void setFont(Font *font);

		/**
		 * @brief Get the current font of this label.
		 */
		inline const Font *getFont() const {
			return font;
		}
		
		/**
		 * @brief Set the text to be displayed.
		 * @param text
		 */
		void setText(const std::string &text);
		
		/**
		 * @brief Get the current text of this node.
		 */
		inline const std::string &getText() const {
			return text;
		}

		/**
		 * @brief Set the align of the text within this node.
		 * @param align		The text's align:
		 *					0.0 means left aligned.
		 *					0.5 means centered.
		 *					1.0 means right aligned.
		 */
		void setTextAlign(float align);

		/**
		 * @brief Get the current align of this label.
		 */
		inline float getTextAlign() const {
			return align;
		}

	protected:
		/**
		 * @brief Update the label's content, if possible.
		 */
		void updateLabelContent();

	private:
		Font*			font;

		std::string		text;
		float			align;
	};

}

#endif	// __WIESEL_UI_LABELNODE_H__


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
#ifndef __WIESEL_UI_FONT_H__
#define	__WIESEL_UI_FONT_H__

#include <wiesel/wiesel-common.def>

#include "wiesel/util/shared_object.h"

#include <string>
#include <vector>


namespace wiesel {

	// predeclaration

	class MultiSpriteNode;


	/**
	 * @brief A class used by \ref LabelNode
	 */
	class WIESEL_COMMON_EXPORT Font : public virtual SharedObject
	{
	protected:
		Font();

	private:
		Font(const Font&) {};

	public:
		virtual ~Font();

	public:
		/**
		 * @brief This font will be assigned to a specific node.
		 * The font object may perform some initialization here.
		 * @param node	The node this font will be assigned to.
		 */
		virtual void onAssignToNode(MultiSpriteNode *node) const;

		/**
		 * @brief This font will be deassigned from a node, which has previously used this font.
		 * The font object may undo some settings, which were initialized in onAssignToNode.
		 * @param node	The node this font will be unassigned from.
		 */
		virtual void onDeassignFromNode(MultiSpriteNode *node) const;

		/**
		 * @brief Measure a character's width.
		 */
		virtual float getCharWidth(char c) const = 0;

		/**
		 * @brief Measure a string's width.
		 */
		virtual float getStringWidth(const std::string &text) const;


		/**
		 * @brief Measure a character's height.
		 */
		virtual float getCharHeight(char c) const = 0;

		/**
		 * @brief Measure a string's height.
		 */
		virtual float getStringHeight(const std::string &text) const;


		/**
		 * @brief Fills a Label node with image content to display the given text.
		 * The text may contain linebreaks.
		 * @param msn		A MultiSpriteNode (usually a LabelNode), which should display the text.
		 * @param text		The text to be displayed.
		 * @param align		A float value controlling the alignment of the text.
		 *						0.0 means left aligned
		 *						0.5 means centered
		 *						1.0 means right aligned
		 */
		virtual void fillLabelNode(
						MultiSpriteNode *msn,
						const std::string &text,
						float align=0.0f
		) const;

		/**
		 * @brief Fills a Label node with image content to display the given text.
		 * @param msn		A MultiSpriteNode (usually a LabelNode), which should display the text.
		 * @param text		A vector containing mulitple lines of text.
		 * @param align		A float value controlling the alignment of the text.
		 *						0.0 means left aligned
		 *						0.5 means centered
		 *						1.0 means right aligned
		 */
		virtual void fillLabelNode(
						MultiSpriteNode *msn,
						const std::vector<std::string> &text,
						float align=0.0f
		) const = 0;

	};

}

#endif	// __WIESEL_UI_FONT_H__


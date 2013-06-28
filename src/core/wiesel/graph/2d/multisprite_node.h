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
#ifndef __WIESEL_GRAPH_2D_MULTISPRITENODE_H__
#define	__WIESEL_GRAPH_2D_MULTISPRITENODE_H__

#include <wiesel/wiesel-core.def>

#include "node2d.h"
#include "sprite_node.h"
#include "wiesel/video/texture.h"
#include "wiesel/video/texture_target.h"
#include "wiesel/video/vertexbuffer.h"
#include "wiesel/geometry.h"

#include <list>


namespace wiesel {

	// predeclaration

	class SpriteFrame;



	/**
	 * @brief A Node for displaying multiple sprites.
	 * Using a MultiSpriteNode instead of multiple, single SpriteNode objects
	 * will be more performant, because the MultiSpriteNode will render them at once
	 * in a single drawing call.
	 * All SpriteFrames within this node needs to be part of the same texture.
	 *
	 * This Node is designed to provide a very basic functionality, so each sprite can
	 * have an individual offset, but no custom transformations like scaling or rotation.
	 */
	class WIESEL_CORE_EXPORT MultiSpriteNode :
			public Node2D,
			public video::ShaderTarget,
			public video::SingleTextureTarget
	{
	public:
		/**
		 * @brief Constructs a new MultiSpriteNode without any texture assigned to it.
		 */
		MultiSpriteNode();

		/**
		 * @brief Constructs a new MultiSpriteNode and assigns a texture to it.
		 * @param texture	A valid texture.
		 */
		MultiSpriteNode(video::Texture *texture);

		virtual ~MultiSpriteNode();

	public:
		/**
		 * @brief An entry within the MultiSpriteNode, which contains a single SpriteFrame with an individual offset.
		 */
		struct Entry {
			/// A single SpriteFrame, which should be drawn.
			SpriteFrame*	sprite;

			/// The offset of this frame, relative to the MultiSpriteNode's bottom-left corner.
			vector2d		offset;
		};

		/// Alias type for a list of entries.
		typedef std::list<Entry>			EntryList;

		/// Alias type for an entries index within this list.
		typedef EntryList::size_type		index_t;

	// getter / setter
	public:
		/**
		 * @brief Set the hit detection method for this sprite.
		 */
		void setSpriteHitDetection(SpriteHitDetection hit);

		/**
		 * @brief Get the hit detection method, currently used by this sprite.
		 */
		inline SpriteHitDetection getSpriteHitDetection() const {
			return hit_detection;
		}

	// managing entries
	public:
		/**
		 * @brief Adds a new sprite with an optional offset to this node.
		 * @param sprite	A new sprite.
		 * @param offset	An optional offset of this sprite, relative to this node's bottom left corner.
		 * @return			The index of the new sprite.
		 */
		index_t addSprite(SpriteFrame *sprite, const vector2d &offset = vector2d::zero);

		/**
		 * @brief Adds a new sprite with an optional offset to this node.
		 * @param sprite	A new sprite.
		 * @param offset_x	An optional offset of this sprite, relative to this node's bottom left corner.
		 * @param offset_y	An optional offset of this sprite, relative to this node's bottom left corner.
		 * @return			The index of the new sprite.
		 */
		index_t addSprite(SpriteFrame *sprite, float offset_x, float offset_y);

		/**
		 * @brief Removes the sprite at the given index from the list.
		 * @param idx		The index of the sprite to be removed.
		 */
		void removeSprite(index_t idx);

		/**
		 * @brief Removes all entries from the list.
		 */
		void clear();

		/**
		 * @brief Get the list of all entries within this node.
		 */
		inline const EntryList *getEntries() const {
			return &entries;
		}

	// node2s stuff
	public:
		virtual bool hitBy(const vector2d &local) const;

	// TextureTarget
	protected:
		virtual void onTextureChanged(uint16_t index, video::Texture *old_texture, video::Texture *new_texture);

	// overridables
	protected:
		virtual void onDraw(video::RenderContext *render_context);

		virtual void rebuildVertexBuffer();

		virtual void updateBounds();

	private:
		SpriteHitDetection		hit_detection;

		EntryList				entries;

		video::IndexBuffer*		indices;
		video::VertexBuffer*	vbo;
		bool					vbo_dirty;
	};

}

#endif	/* __WIESEL_GRAPH_2D_MULTISPRITENODE_H__ */


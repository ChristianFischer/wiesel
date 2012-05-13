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
#ifndef __WIESEL_GRAPH_2D_SPRITENODE_H__
#define	__WIESEL_GRAPH_2D_SPRITENODE_H__

#include "node2d.h"
#include "wiesel/gl/texture/texture.h"
#include "wiesel/gl/vbo/vertexbuffer.h"
#include "wiesel/geometry.h"


namespace wiesel {


	/**
	 * @brief A Node for displaying a single sprite.
	 */
	class SpriteNode : public Node2D
	{
	public:
		/**
		 * @brief Constructs a new, empty SpriteNode
		 */
		SpriteNode();

		/**
		 * @brief Constructs a new SpriteNode containing the full image
		 * of a given texture and a default shader.
		 * @param texture	A valid texture.
		 */
		SpriteNode(Texture *texture);

		/**
		 * @brief Constructs a new SpriteNode containing the full image
		 * of a given texture and a default shader.
		 * @param texture		A valid texture.
		 * @param texture_rect	The area in the texture, which will be drawn.
		 */
		SpriteNode(Texture *texture, const rect &texture_rect);

		virtual ~SpriteNode();

	// getter / setter
	public:
		/**
		 * @brief Set the Shader to be used for rendering.
		 */
		void setShader(ShaderProgram *shader);

		/**
		 * @brief Set the texture to be rendered.
		 */
		void setTexture(Texture *texture);

		/**
		 * @brief Set the area within the texture, which will be drawn.
		 */
		void setTextureRect(const rect &texture_rect);

		/**
		 * @brief Get the currently used shader.
		 */
		inline ShaderProgram *getShader() {
			return shader;
		}

		/**
		 * @brief Get the currently used texture.
		 */
		inline Texture *getTexture() {
			return texture;
		}

		/**
		 * @brief Get the area within the texture, which will be drawn.
		 */
		inline const rect& getTextureRect() const {
			return texture_rect;
		}

	// overridables
	protected:
		virtual void onDraw();

		virtual void rebuildVertexBuffer();

	protected:
		Texture*			texture;
		rect				texture_rect;

		ShaderProgram*		shader;
		VertexBuffer*		vbo;
		bool				vbo_dirty;
	};

}

#endif	/* __WIESEL_GRAPH_2D_SPRITENODE_H__ */

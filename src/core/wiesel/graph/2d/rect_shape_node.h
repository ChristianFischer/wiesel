/**
 * Copyright (C) 2013
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
#ifndef __WIESEL_GRAPH_2D_RECT_SHAPE_NODE_H__
#define __WIESEL_GRAPH_2D_RECT_SHAPE_NODE_H__

#include <wiesel/wiesel-core.def>

#include "node2d.h"
#include "wiesel/video/shader.h"
#include "wiesel/video/shader_target.h"
#include "wiesel/video/vertexbuffer.h"
#include "wiesel/geometry.h"

namespace wiesel {

	/**
	 * @brief A node displaying a single colored rectangle.
	 */
	class WIESEL_CORE_EXPORT RectShapeNode : public Node2D, public video::ShaderTarget
	{
	public:
		/**
		 * @brief Creates a new rectangle node.
		 */
		RectShapeNode(const vector2d& position, const dimension& size);

		/**
		 * @brief Creates a new rectangle node.
		 */
		explicit RectShapeNode(const rectangle& rect);

		/**
		 * @brief Creates a new rectangle node.
		 */
		explicit RectShapeNode(const dimension& size);

		/**
		 * @brief Creates a new rectangle node without size and color.
		 */
		RectShapeNode();

		/**
		 * @brief Creates a new rectangle node.
		 */
		RectShapeNode(float w, float h);

		/**
		 * @brief Creates a new rectangle node.
		 */
		RectShapeNode(float x, float y, float w, float h);

		virtual ~RectShapeNode();

	public:
		/**
		 * @brief Set the rectangle bounds for this node.
		 */
		void setRect(const rectangle& rect);

		/**
		 * @brief Set the rectangle bounds for this node.
		 */
		void setRect(const dimension& size);

		/**
		 * @brief Set the rectangle bounds for this node.
		 */
		void setRect(const vector2d& position, const dimension& size);

		/**
		 * @brief Set the rectangle bounds for this node.
		 */
		void setRect(float w, float h);

		/**
		 * @brief Set the rectangle bounds for this node.
		 */
		void setRect(float x, float y, float w, float h);

	public:
		/**
		 * @brief Set the color of all vertices.
		 */
		void setColor(float r, float g, float b, float a=1.0f);

		/**
		 * @brief Set the color of the top-left vertex.
		 */
		void setColorTopLeft(float r, float g, float b, float a=1.0f);

		/**
		 * @brief Set the color of the top-right vertex.
		 */
		void setColorTopRight(float r, float g, float b, float a=1.0f);

		/**
		 * @brief Set the color of the bottom-left vertex.
		 */
		void setColorBottomLeft(float r, float g, float b, float a=1.0f);

		/**
		 * @brief Set the color of the bottom-right vertex.
		 */
		void setColorBottomRight(float r, float g, float b, float a=1.0f);

	public:
		virtual void onDraw(video::RenderContext *render_context);

	private:
		/**
		 * @brief Ensure a vertex buffer with four vertices exists.
		 */
		void ensureVertexBufferExists();

	private:
		ref<video::VertexBuffer>	vbo;
	};

}

#endif // __WIESEL_GRAPH_2D_RECT_SHAPE_NODE_H__

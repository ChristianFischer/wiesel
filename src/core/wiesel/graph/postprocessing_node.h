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
#ifndef __WIESEL_GRAPH_POSTPROCESSING_NODE_H__
#define __WIESEL_GRAPH_POSTPROCESSING_NODE_H__

#include "render_buffer_node.h"

#include "wiesel/video/shader_target.h"
#include "wiesel/video/vertexbuffer.h"


namespace wiesel {

	/**
	 * @brief A node which can be used to draw its children into a given renderbuffer.
	 * Afterwards the result will be drawn on the screen by using the attached shader
	 * for performing postprocessing effects.
	 */
	class WIESEL_CORE_EXPORT PostProcessingNode :
				public RenderBufferNode,
				public video::ShaderTarget
	{
	public:
		/**
		 * @brief Creates a new, empty \c PostProcessingNode without a \ref RenderBuffer object.
		 */
		PostProcessingNode();

		/**
		 * @brief Creates a new \c PostProcessingNode with the given \ref RenderBuffer object.
		 */
		PostProcessingNode(wiesel::video::RenderBuffer *render_buffer);

		/**
		 * @brief Creates a new \c PostProcessingNode with a new \ref RenderBuffer
		 * based on the given texture.
		 * @param target_texture	A texture which will receive the drawing results.
		 */
		PostProcessingNode(wiesel::video::Texture *target_texture);

		virtual ~PostProcessingNode();

	public:
		virtual void render(video::RenderContext *render_context);

	protected:
		virtual void onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport);

	protected:
		/**
		 * @brief Renders the content of this node's renderbuffer.
		 */
		virtual void renderBufferContent(video::RenderContext *render_context);

		/**
		 * @brief Update the \ref VertexBuffer which is used to render the result of this node.
		 * @param size	The vertexbuffer's size.
		 */
		virtual void updateVertexBuffer(const rectangle& size);

	private:
		ref<video::VertexBuffer>	vbo;
	};

}

#endif // __WIESEL_GRAPH_POSTPROCESSING_NODE_H__

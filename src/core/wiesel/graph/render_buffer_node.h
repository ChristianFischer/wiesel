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
#ifndef __WIESEL_GRAPH_RENDERBUFFER_NODE_H__
#define __WIESEL_GRAPH_RENDERBUFFER_NODE_H__

#include "viewport.h"

#include "wiesel/video/render_buffer.h"


namespace wiesel {

	/**
	 * @brief A node which can be used to draw its children into a given renderbuffer.
	 * The drawing result will not be displayed on the screen
	 */
	class WIESEL_CORE_EXPORT RenderBufferNode : public Viewport
	{
	public:
		/**
		 * @brief Creates a new, empty \c RenderBufferNode without a \ref RenderBuffer object.
		 */
		RenderBufferNode();

		/**
		 * @brief Creates a new \c RenderBufferNode with the given \ref RenderBuffer object.
		 */
		RenderBufferNode(wiesel::video::RenderBuffer *render_buffer);

		/**
		 * @brief Creates a new \c RenderBufferNode with a new \ref RenderBuffer
		 * based on the given texture.
		 * @param target_texture	A texture which will receive the drawing results.
		 */
		RenderBufferNode(wiesel::video::Texture *target_texture);

		virtual ~RenderBufferNode();

	// getter / setter
	public:
		/**
		 * @brief Set the \ref RenderBuffer for this node.
		 */
		void setRenderBuffer(video::RenderBuffer *render_buffer);

		/**
		 * @brief Get this node's assigned \ref RenderBuffer.
		 */
		inline video::RenderBuffer* getRenderBuffer() {
			return render_buffer;
		}

		/**
		 * @brief Get this node's assigned \ref RenderBuffer.
		 */
		inline const video::RenderBuffer* getRenderBuffer() const {
			return render_buffer;
		}

	// Viewport
	protected:
		virtual void onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport);

	// Node
	public:
		virtual void render(video::RenderContext *render_context);

	private:
		ref<wiesel::video::RenderBuffer>	render_buffer;
	};

}

#endif // __WIESEL_GRAPH_RENDERBUFFER_NODE_H__

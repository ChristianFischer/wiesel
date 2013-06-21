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
#include "render_buffer_node.h"
#include "wiesel/video/render_context.h"

using namespace wiesel;


RenderBufferNode::RenderBufferNode() {
}


RenderBufferNode::RenderBufferNode(video::RenderBuffer* render_buffer) {
	setRenderBuffer(render_buffer);
}


RenderBufferNode::RenderBufferNode(video::Texture* target_texture) {
	setRenderBuffer(new video::RenderBuffer(target_texture));
}


RenderBufferNode::~RenderBufferNode() {
}


void RenderBufferNode::setRenderBuffer(video::RenderBuffer* render_buffer) {
	this->render_buffer = render_buffer;
}


void RenderBufferNode::onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport) {
	Viewport::onViewportChanged(old_viewport, new_viewport);
}


void RenderBufferNode::render(video::RenderContext* render_context) {
	if (render_buffer) {
		if (render_buffer->getContent() == NULL) {
			render_buffer->loadContentFrom(render_context->getScreen());
		}

		render_context->pushRenderBuffer(render_buffer);
		Viewport::render(render_context);
		render_context->popRenderBuffer(render_buffer);
	}

	return;
}

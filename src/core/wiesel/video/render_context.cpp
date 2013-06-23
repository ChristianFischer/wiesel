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
#include "render_context.h"
#include "render_buffer.h"
#include "shaders.h"

using namespace wiesel;
using namespace wiesel::video;



RenderContext::RenderContext() {
	this->screen = NULL;
	return;
}


RenderContext::RenderContext(Screen *screen) {
	this->screen = screen;

	return;
}


RenderContext::~RenderContext() {
	return;
}


bool RenderContext::pushRenderBuffer(RenderBuffer* render_buffer) {
	if (renderbuffer_stack.empty() || renderbuffer_stack.top() != render_buffer) {
		if (render_buffer->getContent() && render_buffer->getContent()->enableRenderBuffer(this)) {
			render_buffer->getContent()->preRender(this);
			renderbuffer_stack.push(keep(render_buffer));
			active_renderbuffer = keep(render_buffer);
			return true;
		}
	}

	return false;
}


void RenderContext::popRenderBuffer(RenderBuffer* render_buffer) {
	if (renderbuffer_stack.empty() == false) {
		assert(renderbuffer_stack.top() == render_buffer);

		if (renderbuffer_stack.top() == render_buffer) {
			if (render_buffer->getContent()) {
				render_buffer->getContent()->postRender(this);
				render_buffer->getContent()->disableRenderBuffer(this);
			}

			release(renderbuffer_stack.top());
			renderbuffer_stack.pop();

			clear_ref(active_renderbuffer);
		}

		// when there are still renderbuffers available, enable the top one
		if (renderbuffer_stack.empty() == false) {
			if (renderbuffer_stack.top()->getContent()) {
				if (renderbuffer_stack.top()->getContent()->enableRenderBuffer(this)) {
					active_renderbuffer = keep(renderbuffer_stack.top());
				}
			}
		}
	}

	return;
}

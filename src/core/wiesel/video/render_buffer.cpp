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
#include "render_buffer.h"
#include "video_driver.h"

using namespace wiesel;
using namespace wiesel::video;


RenderBuffer::RenderBuffer() {
}


RenderBuffer::RenderBuffer(Texture* texture) {
	setTargetTexture(0, texture);
	setViewport(texture->getOriginalSize());
}


RenderBuffer::~RenderBuffer() {
	clearTextures();
}


void RenderBuffer::setViewport(const rectangle& viewport) {
	this->viewport = viewport;
}


void RenderBuffer::setViewport(const dimension& size) {
	this->viewport.position = vector2d::zero;
	this->viewport.size     = size;
}


void RenderBuffer::setViewport(float x, float y, float width, float height) {
	this->viewport.position.x  = x;
	this->viewport.position.y  = y;
	this->viewport.size.width  = width;
	this->viewport.size.height = height;
}


void RenderBuffer::setTargetTexture(uint8_t index, Texture* texture) {
	if (target_textures.size() <= index) {
		TextureEntry new_entry;
		new_entry.texture = NULL;

		target_textures.resize(index + 1, new_entry);
	}

	if (target_textures[index].texture != texture) {
		if (target_textures[index].texture) {
			clear_ref(target_textures[index].texture);
		}

		if (texture) {
			target_textures[index].texture = keep(texture);
		}
	}

	return;
}


Texture *RenderBuffer::getTargetTexture(uint8_t index) const {
	if (index < target_textures.size()) {
		return target_textures[index].texture;
	}

	return NULL;
}


void RenderBuffer::clearTextures() {
	for(TextureList::iterator it=target_textures.begin(); it!=target_textures.end(); ++it) {
		if (it->texture) {
			clear_ref(it->texture);
		}
	}

	return;
}


bool RenderBuffer::doLoadContent() {
	assert(getContent() == NULL);

	// get the video device
	Screen *screen = dynamic_cast<Screen*>(getDevice());
	if (screen == NULL) {
		return false;
	}

	VideoDeviceDriver *driver = screen->getVideoDeviceDriver();
	if (driver == NULL) {
		return false;
	}

	RenderBufferContent *rc = driver->createRenderBufferContent(this);
	if (rc == NULL) {
		return false;
	}

	setContent(rc);

	return true;
}


bool RenderBuffer::doUnloadContent() {
	setContent(NULL);
	return true;
}





RenderBufferContent::RenderBufferContent() {
	this->render_buffer = NULL;
}

RenderBufferContent::RenderBufferContent(RenderBuffer* render_target) {
	this->render_buffer = render_target;
}

RenderBufferContent::~RenderBufferContent() {
	return;
}

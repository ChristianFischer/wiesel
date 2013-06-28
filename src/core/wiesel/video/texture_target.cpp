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
#include "texture_target.h"

#include "render_context.h"

using namespace wiesel;
using namespace wiesel::video;


TextureTarget::TextureTarget() {
}

TextureTarget::~TextureTarget() {
}


void TextureTarget::onTextureChanged(uint16_t index, Texture *old_texture, Texture *new_texture) {
}





SingleTextureTarget::SingleTextureTarget() {
}

SingleTextureTarget::~SingleTextureTarget() {
}


void SingleTextureTarget::setTexture(Texture* texture) {
	if (this->texture != texture) {
		onTextureChanged(0, this->texture, this->texture = texture);
	}
}


void SingleTextureTarget::setTexture(uint16_t index, Texture *texture) {
	if (index == 0) {
		this->setTexture(texture);
	}
}


Texture* SingleTextureTarget::getTexture(uint16_t index) {
	if (index == 0) {
		return getTexture();
	}

	return NULL;
}


const Texture* SingleTextureTarget::getTexture(uint16_t index) const {
	if (index == 0) {
		return getTexture();
	}

	return NULL;
}


uint16_t SingleTextureTarget::getTextureLayers() const {
	return 1;
}


uint16_t SingleTextureTarget::getTextureLayersMax() const {
	return 1;
}


void SingleTextureTarget::applyTextureConfigTo(RenderContext* rc) {
	rc->prepareTextureLayers(1);
	rc->setTexture(0, texture);
}





MultiTextureTarget::MultiTextureTarget() {
}

MultiTextureTarget::~MultiTextureTarget() {
}


void MultiTextureTarget::setTexture(uint16_t index, Texture *texture) {
	if (index < getTextureLayersMax()) {
		ref<Texture> old = getTexture(index);

		if (texture != old) {
			if (index >= textures.size()) {
				textures.resize(index + 1, NULL);
			}

			textures[index] = texture;
			onTextureChanged(index, old, texture);
		}
	}
}


Texture* MultiTextureTarget::getTexture(uint16_t index) {
	if (index < textures.size()) {
		return textures[index];
	}

	return NULL;
}


const Texture* MultiTextureTarget::getTexture(uint16_t index) const {
	if (index < textures.size()) {
		return textures[index];
	}

	return NULL;
}


uint16_t MultiTextureTarget::getTextureLayers() const {
	return static_cast<uint16_t>(textures.size());
}


void MultiTextureTarget::applyTextureConfigTo(RenderContext* rc) {
	rc->prepareTextureLayers(textures.size());
	uint16_t count = getTextureLayers();

	for(uint16_t layer=0; layer<count; layer++) {
		rc->setTexture(layer, textures[layer]);
	}

	return;
}


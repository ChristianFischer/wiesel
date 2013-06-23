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
#include "multisprite_node.h"
#include "wiesel/engine.h"
#include "wiesel/resources/graphics/spriteframe.h"
#include "wiesel/video/indexbuffer.h"
#include "wiesel/video/render_context.h"
#include "wiesel/video/shaders.h"
#include "wiesel/video/video_driver.h"

using namespace wiesel;
using namespace wiesel::video;


MultiSpriteNode::MultiSpriteNode() {
	this->texture		= NULL;
	this->indices		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	return;
}


MultiSpriteNode::MultiSpriteNode(Texture *texture) {
	this->texture		= NULL;
	this->indices		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setTexture(texture);

	return;
}


MultiSpriteNode::~MultiSpriteNode() {
	clear_ref(vbo);

	setTexture(NULL);
	clear();

	return;
}


void MultiSpriteNode::setTexture(Texture* texture) {
	clear_ref(this->texture);

	if (texture) {
		this->texture = keep(texture);

		// check, if the new texture is different to
		// the texture used in the sprite frames
		if (
				entries.size()
			&&	entries.front().sprite
			&&	entries.front().sprite->getTexture() != texture
		) {
			// we have to clear the sprite list
			clear();
		}
	}

	return;
}


MultiSpriteNode::index_t MultiSpriteNode::addSprite(SpriteFrame* sprite, float offset_x, float offset_y) {
	return addSprite(sprite, vector2d(offset_x, offset_y));
}


MultiSpriteNode::index_t MultiSpriteNode::addSprite(SpriteFrame* sprite, const vector2d &offset) {
	assert(sprite);
	if (sprite == NULL) {
		return 0;
	}

	// when no texture is set for this node, use the sprite's texture.
	if (this->getTexture() == NULL) {
		setTexture(sprite->getTexture());
	}

	// check, if the sprite's texture matches the texture of this node
	assert(sprite->getTexture() == this->getTexture());
	if (sprite->getTexture() == this->getTexture()) {
		index_t idx = entries.size();

		// add a new entry
		Entry entry = { sprite, offset };
		entries.push_back(entry);

		// keep reference
		keep(sprite);

		// update the boundings of this node
		setBounds(createUnion(
					getBounds(),
					rectangle(offset, sprite->getSize())
		));

		// need to update the vertex buffer
		vbo_dirty = true;

		return idx;
	}

	return 0;
}


void MultiSpriteNode::removeSprite(index_t idx) {
	assert(idx < entries.size());
	if (idx < entries.size()) {
		EntryList::iterator it = entries.begin();
		std::advance(it, idx);
		
		// remove sprite reference
		release(it->sprite);

		// remove from list
		entries.erase(it);

		// bounds need to be updated
		updateBounds();
	}

	return;
}


void MultiSpriteNode::clear() {
	for(EntryList::iterator it=entries.begin(); it!=entries.end(); it++) {
		release(it->sprite);
	}

	entries.clear();

	return;
}


bool MultiSpriteNode::hitBy(const vector2d& local) const {
	switch(getSpriteHitDetection()) {
		default:
		case SpriteHitDetection_OuterBounds: {
			for(EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
				vector2d loc = local - it->offset;

				if (
						loc.x >= 0
					&&	loc.y >= 0
					&&	loc.x <= it->sprite->getSize().width
					&&	loc.y <= it->sprite->getSize().height
				) {
					return true;
				}
			}

			break;
		}

		case SpriteHitDetection_InnerBounds: {
			for(EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
				if (it->sprite->getInnerRect().contains(local - it->offset)) {
					return true;
				}
			}

			break;
		}
	}

	return false;
}


void MultiSpriteNode::onDraw(video::RenderContext *render_context) {
	if (texture && !entries.empty()) {
		if (vbo_dirty) {
			rebuildVertexBuffer();
		}

		applyShaderConfigTo(render_context);
		render_context->setModelviewMatrix(getWorldTransform());
		render_context->prepareTextureLayers(1);
		render_context->setTexture(0, texture);

		render_context->draw(video::Triangles, vbo, indices);
	}

	return;
}


void MultiSpriteNode::rebuildVertexBuffer() {
	if (vbo == NULL) {
		vbo = keep(new VertexBuffer());
	}

	if (indices == NULL) {
		indices = keep(new IndexBuffer(2));
	}

	if (vbo_dirty) {
		vbo_dirty = false;

		vbo->clear();
		vbo->setupVertexPositions(2);
		vbo->setupTextureLayer(0);

		indices->clear();

		// we need 4 indices per quad
		if (entries.size() * 4 >= 65536) {
			indices->setBytesPerElement(4);
		}
		else {
			indices->setBytesPerElement(2);
		}

		float texture_w = texture->getSize().width;
		float texture_h = texture->getSize().height;

		for (EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
			SpriteFrame *frame = it->sprite;

			const SpriteFrame::TextureCoords &tex_coords = frame->getTextureCoordinates();
			float sprite_x = frame->getInnerRect().position.x + it->offset.x;
			float sprite_y = frame->getInnerRect().position.y + it->offset.y;
			float sprite_w = frame->getInnerRect().size.width;
			float sprite_h = frame->getInnerRect().size.height;

			VertexBuffer::index_t idx0 = vbo->addVertex(sprite_x,            sprite_y + sprite_h);
			VertexBuffer::index_t idx1 = vbo->addVertex(sprite_x,            sprite_y           );
			VertexBuffer::index_t idx2 = vbo->addVertex(sprite_x + sprite_w, sprite_y + sprite_h);
			VertexBuffer::index_t idx3 = vbo->addVertex(sprite_x + sprite_w, sprite_y           );

			vbo->setVertexTextureCoordinate(idx0, tex_coords.tl.u/texture_w, tex_coords.tl.v/texture_h);
			vbo->setVertexTextureCoordinate(idx1, tex_coords.bl.u/texture_w, tex_coords.bl.v/texture_h);
			vbo->setVertexTextureCoordinate(idx2, tex_coords.tr.u/texture_w, tex_coords.tr.v/texture_h);
			vbo->setVertexTextureCoordinate(idx3, tex_coords.br.u/texture_w, tex_coords.br.v/texture_h);

			indices->addIndex(idx0);
			indices->addIndex(idx1);
			indices->addIndex(idx2);
			indices->addIndex(idx1);
			indices->addIndex(idx3);
			indices->addIndex(idx2);
		}
	}

	if (getShader() == NULL) {
		setShader(Shaders::instance()->getShaderFor(vbo));
	}

	return;
}


void MultiSpriteNode::updateBounds() {
	rectangle bounds;

	for(EntryList::iterator it=entries.begin(); it!=entries.end(); it++) {
		bounds = createUnion(bounds, rectangle(it->offset, it->sprite->getSize()));
	}

	setBounds(bounds);

	return;
}

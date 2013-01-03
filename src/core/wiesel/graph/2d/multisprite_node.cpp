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
#include "wiesel/gl/shader/shaders.h"
#include "wiesel/gl/texture/spriteframe.h"
#include "wiesel/gl/vbo/indexbuffer.h"
#include "wiesel/video/video_device.h"

using namespace wiesel;
using namespace wiesel::video;


MultiSpriteNode::MultiSpriteNode() {
	this->texture		= NULL;
	this->shader		= NULL;
	this->indices		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	return;
}


MultiSpriteNode::MultiSpriteNode(Texture *texture) {
	this->texture		= NULL;
	this->shader		= NULL;
	this->indices		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setTexture(texture);

	return;
}


MultiSpriteNode::~MultiSpriteNode() {
	if (vbo) {
		vbo->release();
	}

	setTexture(NULL);
	clear();

	return;
}


void MultiSpriteNode::setTexture(Texture* texture) {
	if (this->texture) {
		this->texture->release();
		this->texture = NULL;
	}

	if (texture) {
		this->texture = texture;
		this->texture->retain();

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


void MultiSpriteNode::setShader(ShaderProgram* shader) {
	if (this->shader) {
		this->shader->release();
		this->shader = NULL;
	}

	if (shader) {
		this->shader = shader;
		this->shader->retain();
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
		sprite->retain();

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
		it->sprite->release();

		// remove from list
		entries.erase(it);

		// bounds need to be updated
		updateBounds();
	}

	return;
}


void MultiSpriteNode::clear() {
	for(EntryList::iterator it=entries.begin(); it!=entries.end(); it++) {
		it->sprite->release();
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


void MultiSpriteNode::onDraw(video::VideoDevice *video_device) {
	if (texture && !entries.empty()) {
		if (vbo_dirty) {
			rebuildVertexBuffer();
		}

		shader->bind();
		shader->setProjectionMatrix(video_device->getProjectionMatrix());
		shader->setModelviewMatrix(getWorldTransform());

		vbo->bind(shader, texture);
		vbo->render(VertexBuffer_RenderTriangles, indices);
		vbo->unbind(shader);
	}

	return;
}


void MultiSpriteNode::rebuildVertexBuffer() {
	if (vbo == NULL) {
		vbo = new VertexBuffer();
		vbo->retain();
	}

	if (indices == NULL) {
		indices = new IndexBuffer(2);
		indices->retain();
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

		for (EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
			SpriteFrame *frame = it->sprite;

			float sprite_x = frame->getInnerRect().position.x + it->offset.x;
			float sprite_y = frame->getInnerRect().position.y + it->offset.y;
			float sprite_w = frame->getInnerRect().size.width;
			float sprite_h = frame->getInnerRect().size.height;

			VertexBuffer::index_t idx0 = vbo->addVertex(sprite_x,            sprite_y + sprite_h);
			VertexBuffer::index_t idx1 = vbo->addVertex(sprite_x,            sprite_y           );
			VertexBuffer::index_t idx2 = vbo->addVertex(sprite_x + sprite_w, sprite_y + sprite_h);
			VertexBuffer::index_t idx3 = vbo->addVertex(sprite_x + sprite_w, sprite_y           );

			vbo->setVertexTextureCoordinate(idx0, frame->getTextureCoordinates().tl);
			vbo->setVertexTextureCoordinate(idx1, frame->getTextureCoordinates().bl);
			vbo->setVertexTextureCoordinate(idx2, frame->getTextureCoordinates().tr);
			vbo->setVertexTextureCoordinate(idx3, frame->getTextureCoordinates().br);

			indices->addIndex(idx0);
			indices->addIndex(idx1);
			indices->addIndex(idx2);
			indices->addIndex(idx1);
			indices->addIndex(idx3);
			indices->addIndex(idx2);
		}
	}

	if (shader == NULL) {
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

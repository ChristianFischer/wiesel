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
#include "sprite_node.h"
#include "wiesel/engine.h"
#include "wiesel/gl/shader/shaders.h"
#include "wiesel/gl/texture/spriteframe.h"
#include "wiesel/video/video_device.h"

using namespace wiesel;


SpriteNode::SpriteNode() {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	return;
}


SpriteNode::SpriteNode(Texture *texture) {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setTexture(texture);
	setTextureRect(rectangle(texture->getOriginalSize()));

	return;
}


SpriteNode::SpriteNode(Texture *texture, const rectangle &texture_rect) {
	assert(texture);

	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setTexture(texture);
	setTextureRect(texture_rect);

	return;
}


SpriteNode::SpriteNode(SpriteFrame *sprite) {
	assert(sprite);

	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setSpriteFrame(sprite);

	return;
}


SpriteNode::~SpriteNode() {
	if (vbo) {
		vbo->release();
	}

	setTexture(NULL);
	setShader(NULL);

	return;
}


void SpriteNode::setTexture(Texture* texture) {
	if (this->texture) {
		this->texture->release();
		this->texture = NULL;
	}

	if (texture) {
		this->texture = texture;
		this->texture->retain();
	}

	// reset current sprite, when texture was set manually
	if (this->sprite) {
		this->sprite->release();
		this->sprite = NULL;
	}

	return;
}


void SpriteNode::setShader(ShaderProgram* shader) {
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


void SpriteNode::setSpriteFrame(SpriteFrame* sprite) {
	if (this->sprite != sprite) {
		if (this->sprite) {
			this->sprite->release();
			this->sprite = NULL;
			this->setTexture(NULL);
		}

		if (sprite) {
			setTexture(sprite->getTexture());

			this->sprite = sprite;
			this->sprite->retain();

			setBounds(rectangle(sprite->getSize()));
			setTransformDirty();
		}
	}

	return;
}


void SpriteNode::setTextureRect(const rectangle& texture_rect) {
	this->texture_rect = texture_rect;
	this->vbo_dirty    = true;

	setBounds(texture_rect);
	setTransformDirty();

	return;
}


bool SpriteNode::hitBy(const vector2d& local) const {
	switch(getSpriteHitDetection()) {
		default:
		case SpriteHitDetection_OuterBounds: {
			// just test for the node's bounds
			return Node2D::hitBy(local);
		}

		case SpriteHitDetection_InnerBounds: {
			// check the sprite, if any
			if (sprite) {
				return sprite->getInnerRect().contains(local);
			}

			// when a texture is set, check the texture rect
			if (texture) {
				return texture_rect.contains(local);
			}

			break;
		}
	}

	return false;
}


void SpriteNode::onDraw(video::VideoDevice *video_device) {
	if (texture) {
		if (vbo_dirty) {
			rebuildVertexBuffer();
		}

		shader->bind();
		shader->setProjectionMatrix(video_device->getProjectionMatrix());
		shader->setModelviewMatrix(getWorldTransform());

		vbo->bind(shader, texture);
		vbo->render(VertexBuffer_RenderTriangleStrip);
		vbo->unbind(shader);
	}

	return;
}


void SpriteNode::rebuildVertexBuffer() {
	if (vbo == NULL) {
		vbo = new VertexBuffer();
		vbo->retain();
	}

	if (vbo_dirty) {
		vbo_dirty = false;

		if (getSpriteFrame()) {
			float sprite_x = getSpriteFrame()->getInnerRect().position.x;
			float sprite_y = getSpriteFrame()->getInnerRect().position.y;
			float sprite_w = getSpriteFrame()->getInnerRect().size.width;
			float sprite_h = getSpriteFrame()->getInnerRect().size.height;

			vbo->clear();
			vbo->setupVertexPositions(2);
			vbo->setupTextureLayer(0);
			vbo->addVertex(sprite_x,            sprite_y + sprite_h);
			vbo->addVertex(sprite_x,            sprite_y           );
			vbo->addVertex(sprite_x + sprite_w, sprite_y + sprite_h);
			vbo->addVertex(sprite_x + sprite_w, sprite_y           );

			vbo->setVertexTextureCoordinate(0, getSpriteFrame()->getTextureCoordinates().tl);
			vbo->setVertexTextureCoordinate(1, getSpriteFrame()->getTextureCoordinates().bl);
			vbo->setVertexTextureCoordinate(2, getSpriteFrame()->getTextureCoordinates().tr);
			vbo->setVertexTextureCoordinate(3, getSpriteFrame()->getTextureCoordinates().br);
		}
		else {
			float sprite_x   = 0.0f;
			float sprite_y   = 0.0f;
			float sprite_w   = texture_rect.size.width;
			float sprite_h   = texture_rect.size.height;
			float texture_w  = texture->getSize().width;
			float texture_h  = texture->getSize().height;
			float texcoord_l = texture_rect.getMinX() / texture_w;
			float texcoord_t = texture_rect.getMinY() / texture_h;
			float texcoord_r = texture_rect.getMaxX() / texture_w;
			float texcoord_b = texture_rect.getMaxY() / texture_h;

			vbo->clear();
			vbo->setupVertexPositions(2);
			vbo->setupTextureLayer(0);
			vbo->addVertex(sprite_x, sprite_h);
			vbo->addVertex(sprite_x, sprite_y);
			vbo->addVertex(sprite_w, sprite_h);
			vbo->addVertex(sprite_w, sprite_y);

			vbo->setVertexTextureCoordinate(0, 0, texcoord_l, texcoord_t);
			vbo->setVertexTextureCoordinate(1, 0, texcoord_l, texcoord_b);
			vbo->setVertexTextureCoordinate(2, 0, texcoord_r, texcoord_t);
			vbo->setVertexTextureCoordinate(3, 0, texcoord_r, texcoord_b);
		}
	}

	if (shader == NULL) {
		setShader(Shaders::instance()->getShaderFor(vbo));
	}

	return;
}

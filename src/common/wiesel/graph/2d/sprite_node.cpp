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

using namespace wiesel;


SpriteNode::SpriteNode() {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;

	return;
}


SpriteNode::SpriteNode(Texture *texture) {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;

	setTexture(texture);
	setTextureRect(rect(texture->getOriginalSize()));

	return;
}


SpriteNode::SpriteNode(Texture *texture, const rect &texture_rect) {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;

	setTexture(texture);
	setTextureRect(texture_rect);

	return;
}


SpriteNode::SpriteNode(SpriteFrame *sprite) {
	this->sprite		= NULL;
	this->texture		= NULL;
	this->shader		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;

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
			setTextureRect(sprite->getTextureRect());

			this->sprite = sprite;
			this->sprite->retain();
		}
	}

	return;
}


void SpriteNode::setTextureRect(const rect& texture_rect) {
	this->texture_rect = texture_rect;
	this->vbo_dirty    = true;

	setContentSize(texture_rect.size);
	setTransformDirty();

	return;
}


void SpriteNode::onDraw() {
	if (texture) {
		if (vbo_dirty) {
			rebuildVertexBuffer();
		}

		shader->bind();
		shader->setProjectionMatrix(Engine::getCurrent()->getScreen()->getProjectionMatrix());
		shader->setModelviewMatrix(getWorldTransform());

		vbo->bind(shader, texture);
		vbo->render();
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

	if (shader == NULL) {
		setShader(Shaders::instance()->getShaderFor(vbo));
	}

	return;
}

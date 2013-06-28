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
#include "wiesel/resources/graphics/spriteframe.h"
#include "wiesel/video/render_context.h"
#include "wiesel/video/shaders.h"
#include "wiesel/video/video_driver.h"

using namespace wiesel;
using namespace wiesel::video;


SpriteNode::SpriteNode() {
	this->sprite		= NULL;
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	return;
}


SpriteNode::SpriteNode(Texture *texture) {
	this->sprite		= NULL;
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
	this->vbo			= NULL;
	this->vbo_dirty		= true;
	this->hit_detection	= SpriteHitDetection_InnerBounds;

	setSpriteFrame(sprite);

	return;
}


SpriteNode::~SpriteNode() {
	clear_ref(vbo);

	setTexture(NULL);
	setShader(NULL);

	return;
}


void SpriteNode::setSpriteFrame(SpriteFrame* sprite) {
	if (this->sprite != sprite) {
		if (this->sprite) {
			clear_ref(this->sprite);
			this->setTexture(NULL);
		}

		if (sprite) {
			setTexture(sprite->getTexture());

			this->sprite = keep(sprite);

			setBounds(rectangle(sprite->getSize()));
			setTransformDirty();
			vbo_dirty = true;
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
			if (getTexture()) {
				return texture_rect.contains(local);
			}

			break;
		}
	}

	return false;
}


void SpriteNode::onTextureChanged(uint16_t index, Texture *old_texture, Texture *new_texture) {
	SingleTextureTarget::onTextureChanged(index, old_texture, new_texture);

	// reset current sprite, when texture was set manually
	clear_ref(this->sprite);

	// need to update the vertex buffer
	vbo_dirty = true;

	return;
}


void SpriteNode::onDraw(video::RenderContext *render_context) {
	if (getTexture()) {
		if (vbo_dirty) {
			rebuildVertexBuffer();
		}

		this->applyShaderConfigTo(render_context);
		this->applyTextureConfigTo(render_context);

		render_context->setModelviewMatrix(getWorldTransform());
		render_context->draw(video::TriangleStrip, vbo);
	}

	return;
}


void SpriteNode::rebuildVertexBuffer() {
	if (vbo == NULL) {
		vbo = keep(new VertexBuffer());
	}

	if (vbo_dirty) {
		vbo_dirty = false;

		if (getSpriteFrame()) {
			float sprite_x = getSpriteFrame()->getInnerRect().position.x;
			float sprite_y = getSpriteFrame()->getInnerRect().position.y;
			float sprite_w = getSpriteFrame()->getInnerRect().size.width;
			float sprite_h = getSpriteFrame()->getInnerRect().size.height;

			const SpriteFrame::TextureCoords &tex_coords = getSpriteFrame()->getTextureCoordinates();
			float texture_w = getTexture()->getSize().width;
			float texture_h = getTexture()->getSize().height;

			vbo->clear();
			vbo->setupVertexPositions(2);
			vbo->setupTextureLayer(0);
			vbo->addVertex(sprite_x,            sprite_y + sprite_h);
			vbo->addVertex(sprite_x,            sprite_y           );
			vbo->addVertex(sprite_x + sprite_w, sprite_y + sprite_h);
			vbo->addVertex(sprite_x + sprite_w, sprite_y           );

			vbo->setVertexTextureCoordinate(0, tex_coords.tl.u/texture_w, tex_coords.tl.v/texture_h);
			vbo->setVertexTextureCoordinate(1, tex_coords.bl.u/texture_w, tex_coords.bl.v/texture_h);
			vbo->setVertexTextureCoordinate(2, tex_coords.tr.u/texture_w, tex_coords.tr.v/texture_h);
			vbo->setVertexTextureCoordinate(3, tex_coords.br.u/texture_w, tex_coords.br.v/texture_h);
		}
		else {
			float sprite_x   = 0.0f;
			float sprite_y   = 0.0f;
			float sprite_w   = texture_rect.size.width;
			float sprite_h   = texture_rect.size.height;
			float texture_w  = getTexture()->getSize().width;
			float texture_h  = getTexture()->getSize().height;
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

	if (getShader() == NULL) {
		setShader(Shaders::instance()->getShaderFor(vbo));
	}

	return;
}

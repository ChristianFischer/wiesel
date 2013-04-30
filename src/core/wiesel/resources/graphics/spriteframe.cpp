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
#include "spriteframe.h"
#include <wiesel.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace std;


SpriteFrame::SpriteFrame()
: name(""), texture(NULL), inner_rect()
{
	return;
}

SpriteFrame::SpriteFrame(const std::string& name, Texture* texture)
: name(name)
{
	assert(texture);

	if (texture) {
		this->texture     = keep(texture);
		this->inner_rect  = rectangle(texture->getOriginalSize());
		this->sprite_size = texture->getOriginalSize();
		
		float texcoord_l = inner_rect.getMinX();
		float texcoord_t = inner_rect.getMinY();
		float texcoord_r = inner_rect.getMaxX();
		float texcoord_b = inner_rect.getMaxY();
		
		texture_coordinates.tl = vector2d(texcoord_l, texcoord_t);
		texture_coordinates.tr = vector2d(texcoord_r, texcoord_t);
		texture_coordinates.bl = vector2d(texcoord_l, texcoord_b);
		texture_coordinates.br = vector2d(texcoord_r, texcoord_b);
	}
	else {
		texture = NULL;
	}
}

SpriteFrame::SpriteFrame(const std::string& name, Texture* texture, const rectangle& texture_rect)
: name(name), texture(keep(texture)), inner_rect(vector2d::zero, texture_rect.size)
{
	assert(texture);
	assert(texture_rect.position.x + texture_rect.size.width  <= texture->getOriginalSize().width);
	assert(texture_rect.position.y + texture_rect.size.height <= texture->getOriginalSize().height);

	if (texture) {
		sprite_size = texture_rect.size;

		float texcoord_l = texture_rect.getMinX();
		float texcoord_t = texture_rect.getMinY();
		float texcoord_r = texture_rect.getMaxX();
		float texcoord_b = texture_rect.getMaxY();

		texture_coordinates.tl = vector2d(texcoord_l, texcoord_t);
		texture_coordinates.tr = vector2d(texcoord_r, texcoord_t);
		texture_coordinates.bl = vector2d(texcoord_l, texcoord_b);
		texture_coordinates.br = vector2d(texcoord_r, texcoord_b);
	}

	return;
}

SpriteFrame::SpriteFrame(const string& name, Texture* texture,
		const dimension& size, const rectangle& inner_rect, const TextureCoords& texcoords
)
: name(name), texture(keep(texture)), sprite_size(size), inner_rect(inner_rect), texture_coordinates(texcoords)
{
	assert(texture);
	assert(inner_rect.getMaxX() <= size.width);
	assert(inner_rect.getMaxY() <= size.height);

	return;
}

SpriteFrame::~SpriteFrame() {
	safe_release(texture);
}



std::ostream& wiesel::operator <<(std::ostream &o, const SpriteFrame *sprite_frame) {
	o
			<< "SpriteFrame(\""
			<< sprite_frame->getName() << "\": "
			<< sprite_frame->getSize()
			<< ')'
	;

	return o;
}

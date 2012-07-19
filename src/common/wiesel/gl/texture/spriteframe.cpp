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
using namespace std;


SpriteFrame::SpriteFrame()
: name(""), texture(NULL), texture_rect()
{
	return;
}

SpriteFrame::SpriteFrame(const std::string& name, Texture* texture)
: name(name)
{
	assert(texture);

	if (texture) {
		this->texture = texture;
		this->texture->retain();
		this->texture_rect = rect(texture->getOriginalSize());
	}
	else {
		texture = NULL;
	}
}

SpriteFrame::SpriteFrame(const std::string& name, Texture* texture, const rect& texture_rect)
: name(name), texture(texture), texture_rect(texture_rect)
{
	assert(texture);
	assert(texture_rect.position.x + texture_rect.size.width  <= texture->getOriginalSize().width);
	assert(texture_rect.position.y + texture_rect.size.height <= texture->getOriginalSize().height);

	if (texture) {
		texture->retain();
	}
}

SpriteFrame::~SpriteFrame() {
	if (texture) {
		texture->retain();
		texture = NULL;
	}
}



std::ostream& wiesel::operator <<(std::ostream &o, const SpriteFrame *sprite_frame) {
	o
			<< "SpriteFrame(\""
			<< sprite_frame->getName() << "\": "
			<< sprite_frame->getTextureRect()
			<< ')'
	;

	return o;
}

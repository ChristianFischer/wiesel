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
#include "bitmapfont.h"

#include "wiesel/gl/shaders.h"
#include "wiesel/gl/texture/spritesheet.h"
#include "wiesel/graph/2d/multisprite_node.h"

#include "wiesel/util/log.h"

#include <algorithm>


using namespace wiesel;


BitmapFont::BitmapFont() {
	this->shader			= NULL;
	this->spritesheet		= NULL;
	this->char_spacing		= 0.0f;
	this->line_spacing		= 0.0f;
	return;
}

BitmapFont::BitmapFont(SpriteSheet *ss) {
	this->shader			= NULL;
	this->spritesheet		= NULL;
	this->char_spacing		= 0.0f;
	this->line_spacing		= 0.0f;

	setSpriteSheet(ss);

	return;
}

BitmapFont::~BitmapFont() {
	setSpriteSheet(NULL);
	setShader(NULL);
	return;
}


void BitmapFont::setSpriteSheet(SpriteSheet* ss) {
	if (ss != this->spritesheet) {
		if (this->spritesheet) {
			this->spritesheet->release();
			this->spritesheet = NULL;
		}

		if (ss) {
			this->spritesheet = ss;
			this->spritesheet->retain();
		}
	}

	return;
}


void BitmapFont::setShader(ShaderProgram* shader) {
	if (shader != this->shader) {
		if (this->shader) {
			this->shader->release();
			this->shader = NULL;
		}

		if (shader) {
			this->shader = shader;
			this->shader->retain();
		}
	}

	return;
}




void BitmapFont::onAssignToNode(MultiSpriteNode* node) const {
	// when the node does not already have a valid shader,
	// set the preferred shader of this font.
	if (node->getShader() == NULL) {
		node->setShader(this->getShader());
	}

	return;
}


void BitmapFont::onDeassignFromNode(MultiSpriteNode* node) const {
	// when the node's shader was not altered,
	// remove the default shader from this node
	if (node->getShader() == this->getShader()) {
		node->setShader(NULL);
	}

	return;
}


float BitmapFont::getCharWidth(char c) const {
	SpriteFrame *frame = findSpriteForChar(c);
	if (frame) {
		return frame->getSize().width;
	}

	return 0.0f;
}


float BitmapFont::getCharHeight(char c) const {
	SpriteFrame *frame = findSpriteForChar(c);
	if (frame) {
		return frame->getSize().height;
	}

	return 0.0f;
}


void BitmapFont::fillLabelNode(
					MultiSpriteNode* msn,
					const std::vector<std::string>& text,
					float align
) const {
	float max_width = 0.0f;
	float y = 0.0f;

	float line_h  = 0.0f;
	float space_w = 0.0f;
	std::string test = " 0Aa";

	// find some special frames
	SpriteFrame *frame_questionmark	= findSpriteForChar('?');

	// determine "typical" character width and height by using some default characters
	for(std::string::iterator it=test.begin(); it!=test.end(); it++) {
		SpriteFrame *frame = findSpriteForChar(*it);

		if (frame) {
			space_w = std::max(space_w, frame->getSize().width);
			line_h  = std::max(line_h,  frame->getSize().height);
		}
	}

	// when the text-align is set, we need to determine the maximum text width
	if (align != 0.0f) {
		for(std::vector<std::string>::const_iterator it=text.begin(); it!=text.end(); it++) {
			float line_width = getStringWidth(*it);

			if (max_width < line_width) {
				max_width = line_width;
			}
		}
	}

	// start with the bottom line...
	for(std::vector<std::string>::const_reverse_iterator it=text.rbegin(); it!=text.rend(); it++) {
		float x = 0.0f;

		// when the text-align is set, compute the required offset.
		if (align != 0.0f) {
			float line_width = getStringWidth(*it);
			x += ((max_width - line_width) * align);
		}

		// add a frame for each character
		for(std::string::const_iterator s_it=it->begin(); s_it!=it->end(); s_it++) {
			SpriteFrame *frame = findSpriteForChar(*s_it);

			if (frame) {
				msn->addSprite(frame, x, y);
				x += frame->getSize().width;
			}
			else {
				switch(*s_it) {
					case ' ': {
						// space is just a space...
						x += space_w;
						break;
					}

					default: {
						// when the frame was not found, try to replace it with an question mark
						if (frame_questionmark) {
							msn->addSprite(frame_questionmark, x, y);
							x += frame_questionmark->getSize().width;
						}
						else {
							// when no question mark is available, just leave that part empty
							x += space_w;
						}

						break;
					}
				}
			}

			x += getCharSpacing();
		}

		y += getStringHeight(*it);
		y += getLineSpacing();
	}

	return;
}





SpriteFrame *BitmapFont::findSpriteForChar(char c) const {
	assert(getSpriteSheet());
	SpriteFrame *frame = NULL;

	if (getSpriteSheet()) {
		std::string sprite_name(1, c);
		frame = getSpriteSheet()->get(sprite_name);
	}

	return frame;
}
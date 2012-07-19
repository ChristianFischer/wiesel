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
#include "spritesheet.h"
#include "spriteframe.h"

#include <wiesel.h>
#include <wiesel/io/directory.h>
#include <wiesel/io/file.h>
#include <wiesel/util/xml_parser.h>

#include <sstream>


using namespace wiesel;
using namespace std;




enum SpriteSheetParser_State {
	SpriteSheetParser_Null,
	SpriteSheetParser_Finished,

	// simple xml format
	SpriteSheetParser_SimpleXml_TextureAtlas,
	SpriteSheetParser_SimpleXml_Sprite,
};


class SpriteSheetParser : public XmlSaxParser
{
public:
	Directory*					parent_dir;
	SpriteSheetParser_State		state;
	SpriteSheet*				spritesheet;


	SpriteSheetParser(Directory *dir) {
		this->parent_dir	= dir;
		this->state			= SpriteSheetParser_Null;
		this->spritesheet	= NULL;

		return;
	}

	virtual ~SpriteSheetParser() {
		return;
	}

public:
	virtual void onElementStarted(const string &element, const Attributes &attributes) {
		switch(state) {
			case SpriteSheetParser_Null: {
				if (element == "TextureAtlas") {
					state = SpriteSheetParser_SimpleXml_TextureAtlas;

					if (spritesheet == NULL) {
						Attributes::const_iterator texture_path = attributes.find("imagePath");
						Texture *texture = NULL;

						if (texture_path != attributes.end() && parent_dir) {
							File *texture_file = parent_dir->findFile(texture_path->second);

							if (texture_file) {
								texture = Texture::fromFile(texture_file);
							}
						}

						if (texture) {
							spritesheet = new SpriteSheet(texture);
						}
					}
				}

				break;
			}

			case SpriteSheetParser_SimpleXml_TextureAtlas: {
				if (element == "sprite") {
					if (spritesheet) {
						SpriteFrame *sprite = parseSimpleXml_Sprite(attributes);

						if (sprite) {
							spritesheet->add(sprite);
						}
					}
				}

				break;
			}

			default: {
				break;
			}
		}

		return;
	}

	virtual void onElementClosed(const string &element) {
		switch(state) {
			case SpriteSheetParser_SimpleXml_TextureAtlas: {
				if (element == "TextureAtlas") {
					state = SpriteSheetParser_Finished;
				}

				break;
			}

			case SpriteSheetParser_SimpleXml_Sprite: {
				if (element == "sprite") {
					state = SpriteSheetParser_SimpleXml_TextureAtlas;
				}

				break;
			}

			case SpriteSheetParser_Null: {
				break;
			}

			case SpriteSheetParser_Finished: {
				break;
			}
		}

		return;
	}



	SpriteFrame *parseSimpleXml_Sprite(const Attributes &attributes) {
		string	sprite_name			= "";
		int		sprite_texture_x	= 0;
		int		sprite_texture_y	= 0;
		int		sprite_texture_w	= 0;
		int		sprite_texture_h	= 0;

		for(Attributes::const_iterator it=attributes.begin(); it!=attributes.end(); it++) {
			if (it->first == "n") {
				sprite_name = it->second;
				continue;
			}

			if (it->first == "x") {
				stringstream ss(it->second);
				ss >> sprite_texture_x;
				continue;
			}

			if (it->first == "y") {
				stringstream ss(it->second);
				ss >> sprite_texture_y;
				continue;
			}

			if (it->first == "w") {
				stringstream ss(it->second);
				ss >> sprite_texture_w;
				continue;
			}

			if (it->first == "h") {
				stringstream ss(it->second);
				ss >> sprite_texture_h;
				continue;
			}

			// unknown attribute...
		}

		if (
				sprite_name.empty() == false
			&&	sprite_texture_x >= 0
			&&	sprite_texture_y >= 0
			&&	sprite_texture_w >  0
			&&	sprite_texture_h >  0
		) {
			Texture *texture = spritesheet->getTexture();
			assert(texture);

			rect texture_rect(
						sprite_texture_x,
						sprite_texture_y,
						sprite_texture_w,
						sprite_texture_h
			);

			return new SpriteFrame(sprite_name, texture, texture_rect);
		}

		return NULL;
	}
};






SpriteSheet::SpriteSheet()
: texture(NULL)
{
	return;
}

SpriteSheet::SpriteSheet(Texture* texture)
: texture(texture)
{
	assert(texture);

	if (texture) {
		texture->retain();
	}
}

SpriteSheet::~SpriteSheet() {
	if (texture) {
		texture->release();
	}

	return;
}


SpriteSheet *SpriteSheet::fromFile(File* file) {
	SpriteSheetParser parser(file->getParent());
	if (parser.parse(file)) {
		return parser.spritesheet;
	}

	return NULL;
}


void SpriteSheet::add(SpriteFrame *sprite) {
	// does this sprite already exist?
	assert(sprites_by_name.find(sprite->getName()) == sprites_by_name.end());

	// add to the sprite list
	sprite_list.push_back(sprite);
	sprite->retain();

	// add to name mapping
	sprites_by_name.insert(make_pair(sprite->getName(), sprite));
	
	return;
}


void SpriteSheet::addAlias(const std::string& alias, const std::string& original) {
	// does this sprite already exist?
	assert(sprites_by_name.find(alias) == sprites_by_name.end());
	
	// find the original sprite
	NamedList::iterator original_sprite = sprites_by_name.find(alias);
	if (original_sprite != sprites_by_name.end()) {
		sprites_by_name.insert(make_pair(alias, original_sprite->second));
	}
	
	return;
}


void SpriteSheet::addAlias(const std::string& alias, size_t index) {
	// does this sprite already exist?
	assert(sprites_by_name.find(alias) == sprites_by_name.end());

	// is the original available?
	if (sprite_list.size() > index) {
		SpriteFrame *sprite = sprite_list[index];

		// add to the name mapping
		sprites_by_name.insert(make_pair(alias, sprite));
	}

	return;
}


SpriteFrame *SpriteSheet::get(const std::string& name) {
	NamedList::iterator sprite = sprites_by_name.find(name);
	if (sprite != sprites_by_name.end()) {
		return sprite->second;
	}

	return NULL;
}

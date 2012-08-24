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
#include "font.h"


using namespace wiesel;
using namespace std;


Font::Font() {
	return;
}

Font::~Font() {
	return;
}


void Font::onAssignToNode(MultiSpriteNode* node) const {
	return;
}


void Font::onDeassignFromNode(MultiSpriteNode* node) const {
	return;
}


float Font::getStringWidth(const std::string &text) const {
	float width = 0.0f;

	for(string::const_iterator it=text.begin(); it!=text.end(); it++) {
		width += getCharWidth(*it);
	}

	return width;
}


float Font::getStringHeight(const std::string &text) const {
	float height = 0.0f;

	for(string::const_iterator it=text.begin(); it!=text.end(); it++) {
		float char_height = getCharHeight(*it);

		if (height < char_height) {
			height = char_height;
		}
	}

	return height;
}


void Font::fillLabelNode(MultiSpriteNode* msn, const std::string& text, float align) const {
	string::size_type current	= 0;
	string::size_type next		= 0;
	vector<string> lines;

	while((next = text.find('\n', current)) != string::npos) {
		lines.push_back(text.substr(current, next - current));
		current = next + 1;
	}

	if (current) {
		// add the last part of the string
		lines.push_back(text.substr(current));
	}
	else {
		// there was no linebreak in this string, so we can take the full text
		lines.push_back(text);
	}

	fillLabelNode(msn, lines, align);
}


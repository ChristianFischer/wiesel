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
#include "label_node.h"
#include "font.h"

using namespace wiesel;

LabelNode::LabelNode() {
	this->font		= NULL;
	this->align		= 0.0f;
}

LabelNode::~LabelNode() {
	setFont(NULL);
	return;
}



void LabelNode::setFont(Font* font) {
	if (this->font != font) {
		clear_ref(this->font);

		if (font) {
			this->font = keep(font);
		}

		updateLabelContent();
	}

	return;
}


void LabelNode::setText(const std::string& text) {
	if (this->text != text) {
		this->text  = text;

		updateLabelContent();
	}

	return;
}


void LabelNode::setTextAlign(float align) {
	if (this->align != align) {
		this->align  = align;

		updateLabelContent();
	}

	return;
}



void LabelNode::updateLabelContent() {
	this->clear();

	if (
			getFont() != NULL
		&&	getText().empty() == false
	) {
		getFont()->fillLabelNode(this, getText(), getTextAlign());
	}

	return;
}


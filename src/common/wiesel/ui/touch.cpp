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
#include "touch.h"

#include "wiesel/graph/node.h"


using namespace wiesel;


Touch::Touch() {
	return;
}

Touch::Touch(TouchID id, float x, float y) {
	this->id	= id;
	this->age	= 0.0f;
	this->state	= TouchState_Started;
	this->owner	= NULL;
	this->node	= NULL;
	
	// initialize location
	original_screen_location.x = x;
	original_screen_location.y = y;
	updateLocation(x, y);

	return;
}

Touch::~Touch() {
	safe_release(owner);
	safe_release(node);
	return;
}


bool Touch::claim(TouchReceiver* owner) {
	assert(this->owner == NULL);
	assert(owner != NULL);

	if (this->owner == NULL && owner != NULL) {
		this->owner = owner;
		this->owner->retain();

		Node *node = dynamic_cast<Node*>(owner);
		if (node) {
			this->node = node;
			this->node->retain();
		}

		return true;
	}

	return false;
}


void Touch::update(float dt) {
	this->age += dt;
}


void Touch::setState(TouchState state) {
	this->state = state;
}


void Touch::updateLocation(float new_x, float new_y) {
	// store the previous location
	previous_screen_location = current_screen_location;

	// set the new location
	current_screen_location.x = new_x;
	current_screen_location.y = new_y;

	if (node) {
		// TODO: perform transformations based on the node's matrix
	}

	return;
}





TouchReceiver::TouchReceiver() {
	return;
}

TouchReceiver::~TouchReceiver() {
	return;
}

void TouchReceiver::onTouchStarted(Touch*) {
	return;
}

void TouchReceiver::onTouchMoved(const Touch*) {
	return;
}

void TouchReceiver::onTouchEnded(const Touch*) {
	return;
}

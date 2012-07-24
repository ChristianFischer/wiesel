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
#include "touchhandler.h"
#include "wiesel/graph/node.h"
#include "wiesel/engine.h"

using namespace wiesel;




static Node* recursive_findNode(Touch *touch, Node *node) {
	NodeList::const_reverse_iterator it = node->getChildren()->rbegin();

	// ask all children in front
	for(; (it!=node->getChildren()->rend()) && ((*it)->getOrderKey() >= 0); it++) {
		Node *node = recursive_findNode(touch, *it);

		if (node) {
			return node;
		}
	}

	// check, if the current node is hit by the touch
	TouchReceiver *receiver = dynamic_cast<TouchReceiver*>(node);
	if (receiver) {
		receiver->onTouchStarted(touch);

		// check, if the touch was claimed
		if (touch->getOwner()) {
			return node;
		}
	}

	// start from beginning (the list may have been modified!)
	it = node->getChildren()->rbegin();
	for(; (it!=node->getChildren()->rend()) && ((*it)->getOrderKey() >= 0); it++) {
	}

	// ask all children below the current node
	for(; it!=node->getChildren()->rend(); it++) {
		Node *node = recursive_findNode(touch, *it);

		if (node) {
			return node;
		}
	}

	return NULL;
}




TouchHandler::TouchHandler() {
	return;
}

TouchHandler::~TouchHandler() {
	releaseAllTouches();
}


void TouchHandler::startTouch(TouchID id, float x, float y) {
	// usually, there shouldn't be more than one touches with the same ID at the same time.
	assert(findTouchById(id) == NULL);

	// existing touches will be released before starting a new one.
	Touch *previous_touch = findTouchById(id);
	if (previous_touch) {
		releaseTouch(previous_touch->id);
	}

	// so, let's create the new touch object
	Touch *touch = new Touch(id, x, y);

	// store this touch object
	active_touches.push_back(touch);

	Application *app = Engine::getApplication();
	Node  *node  = NULL;

	// search for a node which claims the touch
	for(SceneList::const_reverse_iterator it=app->getSceneStack()->rbegin(); it!=app->getSceneStack()->rend(); it++) {
		node = recursive_findNode(touch, *it);

		if (node) {
			break;
		}
	}

	if (node) {

	}

	return;
}


void TouchHandler::updateTouchLocation(TouchID id, float x, float y) {
	Touch *touch = findTouchById(id);

	if (touch && touch->getOwner()) {
		touch->updateLocation(x, y);

		// send the notification to the TouchReceiver
		if (touch->getOwner()) {
			touch->getOwner()->onTouchMoved(touch);
		}
	}

	return;
}


void TouchHandler::releaseTouch(TouchID id) {
	for(TouchList::iterator it=active_touches.begin(); it!=active_touches.end(); it++) {
		Touch *touch = *it;

		if (touch->id != id) {
			continue;
		}

		// already released?
		if (touch->getState() == TouchState_Released) {
			continue;
		}

		// release the touch
		touch->setState(TouchState_Released);

		// notify the owner
		if (touch->getOwner()) {
			touch->getOwner()->onTouchEnded(*it);
		}
	}

	return;
}


void TouchHandler::releaseAllTouches() {
	for(TouchList::iterator it=active_touches.begin(); it!=active_touches.end(); it++) {
		Touch *touch = *it;

		// touch was released already?
		if (touch->getState() == TouchState_Released) {
			continue;
		}

		releaseTouch(touch->id);
	}

	return;
}


Touch *TouchHandler::findTouchById(TouchID id) {
	for(TouchList::iterator it=active_touches.begin(); it!=active_touches.end(); it++) {
		Touch *touch = *it;

		if (touch->id != id) {
			continue;
		}

		// released touches does not exist any longer
		if (touch->getState() == TouchState_Released) {
			continue;
		}

		return touch;
	}

	return NULL;
}


void TouchHandler::update(float dt) {
	for(TouchList::iterator it=active_touches.begin(); it!=active_touches.end(); it++) {
		Touch *touch = *it;

		// update the touch object
		touch->update(dt);

		switch(touch->getState()) {
			case TouchState_Started: {
				// when touches pressed for a specific amount of time,
				// they will enter the hold-state
				if (touch->getAge() > 1.0f) {
					touch->setState(TouchState_Holding);
				}

				break;
			}

			case TouchState_Released: {
				// clear released touches
				it = active_touches.erase(it);
				--it;

				delete touch;

				break;
			}

			default: {
				// do nothing
				break;
			}
		}
	}

	return;
}


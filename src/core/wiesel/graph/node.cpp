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
#include "node.h"

#include "wiesel/engine.h"

#include <assert.h>
#include <algorithm>


using namespace wiesel;



Node::Node()
 :
	local_transform(matrix4x4::identity),
	world_transform(matrix4x4::identity),
	transform_dirty(true),
	parent(NULL)
{
	return;
}


Node::~Node() {
	assert(parent == NULL);

	// release all remaining children
	for(NodeList::iterator it=children.begin(); it!=children.end(); it++) {
		Node *child = *it;
		child->parent = NULL;
		child->release();
	}

	children.clear();

	return;
}




bool Node::addChild(Node *child, NodeOrder order) {
	bool added = addChildUnsorted(child);
	if (!added) {
		return false;
	}

	// sort the children
	child->order = order;
	sortChildren();

	return true;
}


bool Node::addChildUnsorted(Node* child) {
	assert(child);
	assert(child->parent == NULL);

	NodeList::iterator it = std::find(children.begin(), children.end(), child);
	assert(it == children.end());
	if (child && it != children.end()) {
		// the node was already added to the children list.
		return false;
	}

	children.push_back(child);
	child->parent = this;
	child->retain();

	return true;
}


void Node::removeChild(Node* child) {
	NodeList::iterator it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) {
		children.erase(it);

		assert(child->parent == this);
		child->parent = NULL;

		child->release();
	}

	return;
}


static bool SortChildrenPredicate(const Node *a, const Node *b) {
	return a->getOrderKey() < b->getOrderKey();
}

void Node::sortChildren() {
	std::sort(children.begin(), children.end(), SortChildrenPredicate);
}


void Node::setTransformDirty() {
	transform_dirty = true;

	// set all children "dirty", too
	for(NodeList::iterator it=children.begin(); it!=children.end(); it++) {
		(*it)->setTransformDirty();
	}

	return;
}


void Node::updateTransform() {
	if (getParent() == NULL) {
		world_transform = local_transform;
	}
	else {
		world_transform = local_transform * getParent()->getWorldTransform();
	}

	transform_dirty = false;

	return;
}


vector2d Node::convertScreenToLocal(const vector2d& screen) {
	// at first, convert into OpenGL coordinate space
	vector2d transformed = vector2d(
			+(screen.x / Engine::getCurrent()->getScreen()->getSize().width  - 0.5f) * 2,
			-(screen.y / Engine::getCurrent()->getScreen()->getSize().height - 0.5f) * 2
	);

	// then use the projection- and modelview matrix to get the actual coordinate system
	return
			transformed
		/	Engine::getCurrent()->getScreen()->getProjectionMatrix()
		/	this->getWorldTransform()
	;
}


void Node::render() {
	bool this_drawn = false;

	for(NodeList::iterator it=children.begin(); it!=children.end(); it++) {
		Node *child = *it;

		if (!this_drawn && child->getOrderKey() >= 0) {
			render_this();
		}

		child->render();
	}

	if (!this_drawn) {
		render_this();
	}

	return;
}


void Node::render_this() {
	if (transform_dirty) {
		updateTransform();
	}

	onDraw();

	return;
}


void Node::onDraw() {
	return;
}

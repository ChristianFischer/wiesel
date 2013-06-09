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
#include "wiesel/video/video_driver.h"

#include <assert.h>
#include <algorithm>


using namespace wiesel;



Node::Node()
 :
	local_transform(matrix4x4::identity),
	world_transform(matrix4x4::identity),
	transform_dirty(true),
	visible(true),
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
		release(child);
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

	children.push_back(keep(child));
	child->parent = this;

	return true;
}


void Node::removeChild(Node* child) {
	NodeList::iterator it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) {
		children.erase(it);

		assert(child->parent == this);
		child->parent = NULL;
		release(child);
	}

	return;
}


static bool SortChildrenPredicate(const Node *a, const Node *b) {
	return a->getOrderKey() < b->getOrderKey();
}

void Node::sortChildren() {
	std::sort(children.begin(), children.end(), SortChildrenPredicate);
}


void Node::setVisible(bool visible) {
	this->visible = visible;
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
	// update the parent's transform first, if neccessary
	if (getParent() && getParent()->isTransformDirty()) {
		getParent()->updateTransform();
	}

	// compute the local transform
	local_transform = matrix4x4::identity;
	computeLocalTransform(&local_transform);

	if (getParent() == NULL) {
		world_transform = local_transform;
	}
	else {
		world_transform = local_transform * getParent()->getWorldTransform();
	}

	transform_dirty = false;

	return;
}


void Node::computeLocalTransform(matrix4x4* transform) {
	return;
}


vector2d Node::convertWorldToLocal(const vector2d& world) const {
	// use the modelview matrix to get the actual coordinate system
	return world / this->getWorldTransform();
}


vector3d Node::convertWorldToLocal(const vector3d& world) const {
	// use the modelview matrix to get the actual coordinate system
	return world / this->getWorldTransform();
}


void Node::render(video::RenderContext *render_context) {
	bool this_drawn = false;

	// skip rendering when the node is not visible
	// NOTE: it would be still possible to call onDraw directly
	if (visible == false) {
		return;
	}

	for(NodeList::iterator it=children.begin(); it!=children.end(); it++) {
		Node *child = *it;

		if (!this_drawn && child->getOrderKey() >= 0) {
			render_this(render_context);
			this_drawn = true;
		}

		child->render(render_context);
	}

	if (!this_drawn) {
		render_this(render_context);
	}

	return;
}


void Node::render_this(video::RenderContext *render_context) {
	if (transform_dirty) {
		updateTransform();
	}

	onDraw(render_context);

	return;
}


void Node::onDraw(video::RenderContext *render_context) {
	return;
}

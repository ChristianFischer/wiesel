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
#include "viewport.h"

using namespace wiesel;


Viewport *Viewport::findFrom(Node *node) {
	if (node == NULL) {
		return NULL;
	}

	Viewport *vp = dynamic_cast<Viewport*>(node);
	if (vp) {
		// ensure the viewports transformation is up-to-date before returning it.
		vp->updateTransform();

		return vp;
	}

	return Viewport::findFrom(node->getParent());
}


Viewport::Viewport() {
	this->viewport_flags				= Inherit;
	this->viewport_requested_dimension	= dimension(0, 0);

	setPivot(.0f, .0f);

	return;
}

Viewport::~Viewport() {
	return;
}


void Viewport::setScaleMode(uint32_t flags, const dimension& dim, const vector2d& align) {
	this->viewport_flags				= flags;
	this->viewport_requested_dimension	= dim;
	this->viewport_align				= align;

	setBounds(rectangle(vector2d::zero, dim));
	setTransformDirty();

	return;
}


void Viewport::computeLocalTransform(matrix4x4 *transform) {
	local_transform = matrix4x4::identity;
	rectangle parent_viewport = getParentViewport();
	rectangle new_viewport(vector2d::zero, viewport_requested_dimension);

	if (viewport_flags & Inherit) {
		new_viewport.size = parent_viewport.size;
	}
	else {
		float scale  = 1.0f;
		bool  scaled = false;

		// compute the x scale factor
		if (
				(viewport_flags & ScaleByWidth)
			&&	(parent_viewport.size.width != 0.0f)
		) {
			float scale_x = parent_viewport.size.width / viewport_requested_dimension.width;

			if (!scaled || (scale_x < scale)) {
				scale  = scale_x;
				scaled = true;
			}
		}

		// compute the y scale factor
		if (
				(viewport_flags & ScaleByHeight)
			&&	(parent_viewport.size.height != 0.0f)
		) {
			float scale_y = parent_viewport.size.height / viewport_requested_dimension.height;

			if (!scaled || (scale_y < scale)) {
				scale  = scale_y;
				scaled = true;
			}
		}

		// apply the scale factor
		transform->scale(scale, scale, scale);

		if (viewport_flags & ExtendWidth) {
			float extended = parent_viewport.size.width / scale;
			if (new_viewport.size.width < extended) {
				new_viewport.size.width = extended;
			}
		}

		if (viewport_flags & ExtendHeight) {
			float extended = parent_viewport.size.height / scale;
			if (new_viewport.size.height < extended) {
				new_viewport.size.height = extended;
			}
		}

		// compute the viewports align
		new_viewport.position.x = ((parent_viewport.size.width  / scale) - new_viewport.size.width ) * viewport_align.x;
		new_viewport.position.y = ((parent_viewport.size.height / scale) - new_viewport.size.height) * viewport_align.y;

		transform->translate(
						new_viewport.position.x * transform->get(0, 0),
						new_viewport.position.y * transform->get(1, 1)
		);
	}

	if (this->viewport != new_viewport) {
		this->viewport  = new_viewport;
		onViewportChanged(this->viewport, new_viewport);
	}
	else {
		this->viewport  = new_viewport;
	}

	Node2D::computeLocalTransform(transform);

	return;
}


rectangle Viewport::getParentViewport() {
	Viewport *parent = findFrom(getParent());
	if (parent) {
		return parent->getViewport();
	}

	return viewport;
}


void Viewport::onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport) {
	return;
}

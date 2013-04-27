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
#include "scene.h"
#include "wiesel/video/screen.h"
#include "wiesel/video/video_driver.h"

using namespace wiesel;


Scene::Scene() {
	return;
}

Scene::~Scene() {
	return;
}


void Scene::render(video::RenderContext* render_context) {
	rectangle resolution = rectangle(
			vector2d::zero,
			render_context->getScreen()->getVideoDeviceDriver()->getResolution()
	);

	// update the screen size
	if (this->screen_size != resolution) {
		this->screen_size  = resolution;
		setTransformDirty();
	}

	Viewport::render(render_context);

	return;
}


rectangle Scene::getParentViewport() {
	if (getParent()) {
		return Viewport::getParentViewport();
	}

	return screen_size;
}


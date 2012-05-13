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
#include "application.h"

#include <algorithm>


using namespace wiesel;


Application::Application() {
	return;
}

Application::~Application() {
	clearSceneStack();
	return;
}




void Application::onRun() {
	return;
}


void Application::onRender() {
	// draw all scenes
	for(SceneList::iterator it=scene_stack.begin(); it!=scene_stack.end(); it++) {
		(*it)->render();
	}

	return;
}




bool Application::pushScene(Scene *scene) {
	SceneList::iterator it = std::find(scene_stack.begin(), scene_stack.end(), scene);
	assert(it == scene_stack.end());
	assert(scene);

	if (scene && it == scene_stack.end()) {
		scene_stack.push_back(scene);
		scene->retain();

		return true;
	}

	return false;
}


void Application::removeScene(Scene *scene) {
	SceneList::iterator it = std::find(scene_stack.begin(), scene_stack.end(), scene);
	if (it != scene_stack.end()) {
		scene_stack.erase(it);
		scene->release();
	}

	return;
}


void Application::clearSceneStack() {
	for(SceneList::reverse_iterator it=scene_stack.rbegin(); it!=scene_stack.rend(); it++) {
		(*it)->release();
	}

	scene_stack.clear();

	return;
}


Scene *Application::getTopScene() {
	if (scene_stack.empty()) {
		return NULL;
	}

	return scene_stack.back();
}


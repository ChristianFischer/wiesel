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
#include "engine.h"

#include <wiesel/util/shared_object.h>
#include <wiesel/util/log.h>
#include <wiesel/module.h>
#include <wiesel/module_registry.h>

#include <wiesel/ui/touchhandler.h>

#include <assert.h>
#include <stddef.h>
#include <time.h>

#include <algorithm>


using namespace wiesel;


Engine		Engine::instance;


Engine::Engine() {
	exit_requested		= false;
	application			= NULL;

	return;
}

Engine::~Engine() {
	shutdown();
	return;
}


bool Engine::initialize(int argc, char* argv[]) {
	// try to find a valid platform object
	if (this->platforms.empty()) {
		std::vector<ModuleLoader<Platform>*> loaders = ModuleRegistry::getInstance()->findModules<Platform>();
		for(std::vector<ModuleLoader<Platform>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
			Platform *platform = (*it)->create();

			if (platform) {
				platform->retain();
				platform->onInit();

				this->platforms.push_back(platform);
			}
		}

		assert(platforms.empty() == false);
		if (platforms.empty()) {
			return false;
		}
	}

	return true;
}


FileSystem *Engine::getRootFileSystem() {
	for(std::vector<Platform*>::reverse_iterator it=platforms.rbegin(); it!=platforms.rend(); it++) {
		Platform *platform = *it;

		FileSystem *fs = platform->getRootFileSystem();
		if (fs) {
			return fs;
		}
	}

	return NULL;
}


FileSystem *Engine::getAssetFileSystem() {
	for(std::vector<Platform*>::reverse_iterator it=platforms.rbegin(); it!=platforms.rend(); it++) {
		Platform *platform = *it;

		FileSystem *fs = platform->getAssetFileSystem();
		if (fs) {
			return fs;
		}
	}

	return NULL;
}


bool Engine::shutdown() {
	// release all platforms
	for(std::vector<Platform*>::reverse_iterator it=platforms.rbegin(); it!=platforms.rend(); it++) {
		Platform *platform = *it;
		platform->onShutdown();
		platform->release();
	}

	platforms.clear();

	// done
	return true;
}


void Engine::run() {
	// try to find valid application objects
	std::vector<ModuleLoader<Application>*> loaders = ModuleRegistry::getInstance()->findModules<Application>();
	for(std::vector<ModuleLoader<Application>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		Application *app = (*it)->create();

		// run the main loop with each selected application
		if (app) {
			run(app);
		}
	}

	return;
}


void Engine::run(Application *app) {
	// check, if a platform object is available
	assert(platforms.empty() == false);
	if (platforms.empty()) {
		return;
	}

	// we need a valid application object to run
	assert(app);
	if (app == NULL) {
		return;
	}

	// no other application should be active
	assert(application == NULL);
	if (application) {
		return;
	}

	// initialize the application object
	this->application = app;
	this->application->retain();
	this->application->onInit();

	// reset the exit_requested flag before starting the main loop
	exit_requested = false;

	// first onRun before entering the main loop
	for(std::vector<Platform*>::iterator it=platforms.begin(); it!=platforms.end(); it++) {
		(*it)->onRunFirst();
	}

	// timers
	clock_t now_t = clock();
	clock_t last_t;

	bool done = false;
	do {
		done = false;

		for(std::vector<Platform*>::iterator it=platforms.begin(); it!=platforms.end(); it++) {
			Platform *platform = *it;
			done |= platform->onRun();
		}

		// measure time of this frame
		last_t = now_t;
		now_t  = clock();
		float dt = (float(now_t - last_t) / CLOCKS_PER_SEC);

		// run all updateable objects
		for(int i=updateables.size(); --i>=0;) {
			updateables.at(i)->update(dt);
		}

		// the application's onRun will be invoked every frame
		// the application may decide itself, what to do in each state
		application->onRun(dt);

		// purge all dead objects at the end of each frame
		SharedObject::purgeDeadObjects();

		// exit requested by application?
		done |= exit_requested;
	}
	while(!done);

	// release the application object
	application->onShutdown();
	application->release();
	application = NULL;

	return;
}


void Engine::requestExit() {
	exit_requested = true;
	return;
}


void Engine::registerUpdateable(IUpdateable* updateable) {
	std::vector<IUpdateable*>::iterator it = std::find(updateables.begin(), updateables.end(), updateable);
	if (it == updateables.end()) {
		updateables.push_back(updateable);
		updateable->retain();
	}

	return;
}


void Engine::unregisterUpdateable(IUpdateable* updateable) {
	std::vector<IUpdateable*>::iterator it = std::find(updateables.begin(), updateables.end(), updateable);
	if (it != updateables.end()) {
		updateables.erase(it);
		updateable->release();
	}

	return;
}


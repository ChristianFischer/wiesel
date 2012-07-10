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

#include <wiesel/ui/touchhandler.h>

#include <assert.h>
#include <stddef.h>
#include <time.h>

#include <algorithm>


using namespace wiesel;


Engine*				Engine::current_instance		= NULL;
Application*		Engine::current_app				= NULL;
bool				Engine::exit_requested			= false;


Engine::Engine()
: screen(NULL)
{
	state		= Engine_Uninitialized;
	return;
}

Engine::~Engine() {
	return;
}


bool Engine::install(Engine *engine) {
	// if there's already an existing instance, the installation fails
	assert(current_instance == NULL);
	if (current_instance) {
		return false;
	}

	// initialize the new engine
	if (engine->onInit() == false) {
		return false;
	}

	// store the new instance
	current_instance = engine;

	// success
	return true;
}


bool Engine::shutdown() {
	if (current_instance == NULL) {
		return false;
	}

	// shutdown
	current_instance->onShutdown();
	delete current_instance;
	current_instance = NULL;

	// done
	return true;
}


void Engine::run(Application *application) {
	// no other application running
	assert(current_app == NULL);
	if (current_app) {
		return;
	}

	// we need a valid application object to run
	assert(application);
	if (application == NULL) {
		return;
	}

	// reset the exit_requested flag before starting the main loop
	exit_requested = false;

	// store the application object
	current_app = application;

	// first onRun before entering the main loop
	current_instance->onRunFirst();

	// timers
	clock_t last_t = clock();
	clock_t now_t;

	bool done = false;
	do {
		done = current_instance->onRun();

		// measure time of this frame
		last_t = now_t;
		now_t  = clock();
		float dt = (float(now_t - last_t) / CLOCKS_PER_SEC);

		// run all updateable objects
		for(int i=current_instance->updateables.size(); --i>=0;) {
			current_instance->updateables.at(i)->update(dt);
		}

		switch(current_instance->getState()) {
			case Engine_Uninitialized: {
				// do nothing
				break;
			}

			case Engine_Suspended: {
				// do nothing
				break;
			}

			case Engine_Background: {
				// do nothing
				break;
			}

			case Engine_Running: {
				// application onRun
				current_app->onRun(dt);

				Screen *screen = current_instance->getScreen();

				if (screen) {
					screen->preRender();
					current_app->onRender();
					screen->postRender();
				}

				break;
			}
		}

		// purge all dead objects at the end of each frame
		SharedObject::purgeDeadObjects();

		// exit requested by application?
		done |= exit_requested;
	}
	while(!done);

	// release the current app
	current_app->onShutdown();

	// clear current application
	current_app = NULL;

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





void Engine::startApp() {
	assert(current_instance != NULL);
	assert(current_app != NULL);

	switch(current_instance->getState()) {
		case Engine_Uninitialized: {
			if (current_app) {
				current_app->onInit();
			}

			current_instance->state = Engine_Background;

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::enterBackground() {
	assert(current_instance != NULL);
	assert(current_app != NULL);

	switch(current_instance->getState()) {
		case Engine_Running: {
			if (current_app != NULL) {
				current_app->onEnterBackground();
			}

			current_instance->state = Engine_Background;

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::enterForeground() {
	assert(current_instance != NULL);
	assert(current_app != NULL);

	switch(current_instance->getState()) {
		case Engine_Background: {
			if (current_app != NULL) {
				current_app->onEnterForeground();
			}

			current_instance->state = Engine_Running;

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::suspendApp() {
	assert(current_instance != NULL);
	assert(current_app != NULL);

	switch(current_instance->getState()) {
		case Engine_Running: {
			enterBackground();

			// NOBR
		}

		case Engine_Background: {
			if (current_app) {
				current_app->onSuspend();
			}

			current_instance->state = Engine_Suspended;

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::resumeSuspendedApp() {
	assert(current_instance != NULL);
	assert(current_app != NULL);

	switch(current_instance->getState()) {
		case Engine_Suspended: {
			if (current_app) {
				current_app->onResumeSuspended();
			}

			current_instance->state = Engine_Background;

			break;
		}

		default: {
			break;
		}
	}

	return;
}

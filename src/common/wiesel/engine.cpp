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

#include <assert.h>
#include <stddef.h>

using namespace wiesel;


Engine*				Engine::current_instance		= NULL;
Application*		Engine::current_app				= NULL;
ApplicationState	Engine::current_app_state		= Application_Uninitialized;
bool				Engine::exit_requested			= false;


Engine::Engine()
: screen(NULL)
{
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

	bool done = false;
	do {
		done = current_instance->onRun();

		Screen *screen = current_instance->getScreen();

		switch(current_app_state) {
			case Application_Uninitialized: {
				// do nothing
				break;
			}

			case Application_Suspended: {
				// do nothing
				break;
			}

			case Application_Running: {
				// application onRun
				current_app->onRun();

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


void Engine::startApp() {
	assert(current_app != NULL);

	switch(current_app_state) {
		case Application_Uninitialized: {
			if (current_app) {
				current_app->onInit();
				current_app_state = Application_Running;
			}

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::suspendApp() {
	assert(current_app != NULL);

	switch(current_app_state) {
		case Application_Running: {
			if (current_app) {
				current_app->onSuspend();
				current_app_state = Application_Suspended;
			}

			break;
		}

		default: {
			break;
		}
	}

	return;
}


void Engine::resumeSuspendedApp() {
	assert(current_app != NULL);

	switch(current_app_state) {
		case Application_Suspended: {
			if (current_app) {
				current_app->onResumeSuspended();
				current_app_state = Application_Running;
			}

			break;
		}

		default: {
			break;
		}
	}

	return;
}

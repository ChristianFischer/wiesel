/*
 * engine.cpp
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#include "engine.h"

#include <assert.h>
#include <stddef.h>

using namespace wiesel;


Engine*			Engine::current_instance		= NULL;
Application*	Engine::current_app				= NULL;
bool			Engine::exit_requested			= false;


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

	bool done = false;
	do {
		done = current_instance->onRun();

		Screen *screen = current_instance->getScreen();

		// application onRun
		application->onRun();

		if (screen) {
			screen->preRender();
			application->onRender();
			screen->postRender();
		}

		// exit requested by application?
		done |= exit_requested;
	}
	while(!done);

	// clear current application
	current_app = NULL;

	return;
}


void Engine::requestExit() {
	exit_requested = true;
	return;
}


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
#include "sdl2_platform.h"
#include "sdl2_message_receiver.h"

#include <wiesel/engine.h>
#include <wiesel/util/log.h>

#include <SDL.h>

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>
#include <sys/unistd.h>
#include <sys/param.h>
#include <algorithm>
#include <sstream>


using namespace wiesel;
using namespace wiesel::sdl2;
using namespace std;


Sdl2Platform::Sdl2Platform() {
	return;
}

Sdl2Platform::~Sdl2Platform() {
	return;
}


void Sdl2Platform::addReceiver(ISdl2MessageReceiver* recv) {
	receivers.push_back(recv);
	return;
}

void Sdl2Platform::removeReceiver(const ISdl2MessageReceiver* recv) {
	std::vector<ISdl2MessageReceiver*>::iterator it;

	while((it = std::find(receivers.begin(), receivers.end(), recv)) != receivers.end()) {
		receivers.erase(it);
	}

	return;
}


bool Sdl2Platform::onInit() {
	return true;
}


void Sdl2Platform::onShutdown() {
	return;
}


void Sdl2Platform::onRunFirst() {
	return;
}


bool Sdl2Platform::onRun() {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT: {
				return true;
			}

			default: {
				break;
			}
		}

		// send this event to all registered message receivers
		for(std::vector<ISdl2MessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
			(*it)->onEvent(event);
		}
	}

	return false;
}


FileSystem *Sdl2Platform::getRootFileSystem() {
	return NULL;
}


FileSystem *Sdl2Platform::getAssetFileSystem() {
	return NULL;
}

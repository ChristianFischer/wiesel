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
#include "sdl_platform.h"
#include "sdl_message_receiver.h"

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
using namespace wiesel::sdl;
using namespace std;


SdlPlatform::SdlPlatform() {
	return;
}

SdlPlatform::~SdlPlatform() {
	return;
}


void SdlPlatform::addReceiver(ISdlMessageReceiver* recv) {
	receivers.push_back(recv);
	return;
}

void SdlPlatform::removeReceiver(const ISdlMessageReceiver* recv) {
	std::vector<ISdlMessageReceiver*>::iterator it;

	while((it = std::find(receivers.begin(), receivers.end(), recv)) != receivers.end()) {
		receivers.erase(it);
	}

	return;
}


bool SdlPlatform::onInit() {
	return true;
}


void SdlPlatform::onShutdown() {
	return;
}


void SdlPlatform::onRunFirst() {
	return;
}


bool SdlPlatform::onRun() {
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
		for(std::vector<ISdlMessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
			(*it)->onEvent(event);
		}
	}

	return false;
}


FileSystem *SdlPlatform::getRootFileSystem() {
	return NULL;
}


FileSystem *SdlPlatform::getAssetFileSystem() {
	return NULL;
}


FileSystem *SdlPlatform::getDataFileSystem(const std::string &subdir) {
	return NULL;
}


FileSystem *SdlPlatform::getExternalDataFileSystem(const std::string &subdir) {
	return NULL;
}

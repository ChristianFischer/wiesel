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
#include <wiesel/engine.h>

#include "android_platform.h"

#include <android_native_app_glue.h>


using namespace wiesel;
using namespace wiesel::android;


extern "C" void android_main(struct android_app* state) {
	// Make sure glue isn't stripped.
	app_dummy();

	// initialize the engine
	Engine::getInstance()->initialize(0, NULL);

	// find the android platform
	const std::vector<Platform*> *platforms = Engine::getInstance()->getPlatforms();
	for(std::vector<Platform*>::const_iterator it=platforms->begin(); it!=platforms->end(); it++) {
		AndroidPlatform *platform = dynamic_cast<AndroidPlatform*>(*it);
		if (platform) {
			platform->initAndroidApp(state);
		}
	}

	// start the application
	Engine::getInstance()->run();
	Engine::getInstance()->shutdown();

	return;
}


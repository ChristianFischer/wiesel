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
#ifdef __ANDROID__

#include "platform_main.h"
#include "android_engine.h"
#include "android_screen.h"

#include <android_native_app_glue.h>


using namespace wiesel;


void __internal_android_main(Application *app, struct ::android_app* state) {
	// Make sure glue isn't stripped.
	app_dummy();

	// create the android engine object
	AndroidEngine *engine = new AndroidEngine(state);
	Engine::install(engine);
	Engine::run(app);
	Engine::shutdown();

	return;
}


#endif // __ANDROID__

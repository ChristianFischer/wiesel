/*
 * default include file for the android platform
 * platform_main.h
 *
 *  Created on: 23.03.2012
 *      Author: Baldur
 */
#ifndef __WIESEL_PLATFORM_ANDROID_PLATFORM_MAIN_H__
#define __WIESEL_PLATFORM_ANDROID_PLATFORM_MAIN_H__

#include "../../application.h"

#if defined(__ANDROID__)

/// application entry point macro
#define	WIESEL_APPLICATION_SETUP(APPLICATION)						\
	extern "C" void android_main(struct android_app* state) {		\
		APPLICATION app = APPLICATION();							\
		__internal_android_main(&app, state);						\
	}

/// delegate for the application entry point
void __internal_android_main(wiesel::Application *application, struct android_app* state);

#endif // defined(__ANDROID__)

#endif // __WIESEL_PLATFORM_ANDROID_PLATFORM_MAIN_H__

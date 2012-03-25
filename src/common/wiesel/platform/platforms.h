/*
 * main include file for all platforms
 * platforms.h
 *
 *  Created on: 23.03.2012
 *      Author: Baldur
 */
#ifndef __WIESEL_PLATFORM_PLATFORMS_H__
#define __WIESEL_PLATFORM_PLATFORMS_H__

#include "wiesel/application.h"
#include "wiesel/engine.h"


#if defined(__ANDROID__)
	// get the android platform
	#include "android/platform_main.h"
#endif



#endif // __WIESEL_PLATFORM_PLATFORMS_H__

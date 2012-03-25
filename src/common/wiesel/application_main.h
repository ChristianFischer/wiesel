/*
 * application entry point
 * application_main.h
 *
 *  Created on: 23.03.2012
 *      Author: Baldur
 */
#ifndef __WIESEL_APPLICATIONMAIN_H__
#define __WIESEL_APPLICATIONMAIN_H__

#include "../wiesel.h"

// application class
#include "application.h"

// get all the platforms
#include "platform/platforms.h"

// check, if the entry-point macro exists.
#if !defined(WIESEL_APPLICATION_SETUP)
#error "WIESEL_APPLICATION_SETUP not defined - compiling on an unknown platform?"
#endif

#endif // __WIESEL_H__

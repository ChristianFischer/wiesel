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
#ifndef __WIESEL_VIDEO_GLIMPORT_H__
#define __WIESEL_VIDEO_GLIMPORT_H__

#include <wiesel/platform_config.h>

#if WIESEL_PLATFORM_ANDROID

	// android
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>

#else

	// desktop

	// don't define min/max macros - we're using the C++ template functions instead
	#ifndef NOMINMAX
	#define NOMINMAX
	#endif

	// include SDL OpenGL headers
	#include <GLee.h>

#endif


#endif	/* __WIESEL_VIDEO_GLIMPORT_H__ */

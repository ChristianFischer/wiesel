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
#ifndef __WIESEL_PLATFORM_FILEUTILS_H__
#define __WIESEL_PLATFORM_FILEUTILS_H__

#include "wiesel/platform_config.h"

#if WIESEL_PLATFORM_WINDOWS && !WIESEL_PLATFORM_CYGWIN
	#include "windows.h"
#endif

#if WIESEL_PLATFORM_WINDOWS
	#include <direct.h>
#endif

#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
	#include <unistd.h>
#endif

#if !defined(_MSC_VER)
	#include <dirent.h>
#endif

#include <sys/stat.h>

#endif // __WIESEL_PLATFORM_FILEUTILS_H__
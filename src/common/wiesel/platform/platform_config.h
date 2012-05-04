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
#ifndef __WIESEL_PLATFORM_PLATFORM_CONFIG_H__
#define	__WIESEL_PLATFORM_PLATFORM_CONFIG_H__


/// true for any unix based platforms
#define WIESEL_PLATFORM_UNIX				0

/// true when compiling for windows
#define WIESEL_PLATFORM_WINDOWS				0

/// true when compiling on windows via cygwin
#define WIESEL_PLATFORM_CYGWIN				0

/// true when compiling for the android platform
#define WIESEL_PLATFORM_ANDROID				0



#if WIN32 || __WIN32__
#undef  WIESEL_PLATFORM_WINDOWS
#define WIESEL_PLATFORM_WINDOWS				1
#endif

#if linux
#undef  WIESEL_PLATFORM_UNIX
#define WIESEL_PLATFORM_UNIX				1
#endif

#if __CYGWIN__ || __CYGWIN32__
#undef  WIESEL_PLATFORM_CYGWIN
#define WIESEL_PLATFORM_CYGWIN				1
#endif

#if __ANDROID__
#undef  WIESEL_PLATFORM_ANDROID
#define WIESEL_PLATFORM_ANDROID				1
#endif


#endif	/* __WIESEL_PLATFORM_PLATFORM_CONFIG_H__ */


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
#if WIESEL_USE_LIBSDL

#include <wiesel.h>
#include <stdio.h>


using namespace wiesel;


// implementing the platform specific logging
int _logmsg_impl(LogLevel level, const char *tag, const char *message) {
	char lvl = '?';
	switch(level) {
		case LogLevel_Debug:	lvl = 'D';		break;
		case LogLevel_Info:		lvl = 'I';		break;
		case LogLevel_Warning:	lvl = 'W';		break;
		case LogLevel_Error:	lvl = 'E';		break;
		default:				lvl = '-';		break;	// should not happen
	}
	
	printf("[%c] %-16s: %s\n", lvl, tag, message);
	
	return 0;
}

#endif // WIESEL_USE_LIBSDL

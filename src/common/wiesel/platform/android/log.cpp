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
#include <wiesel.h>
#include <android/log.h>


using namespace wiesel;


// implementing the platform specific logging
int _logmsg_impl(LogLevel level, const char *tag, const char *message) {
	android_LogPriority priority = ANDROID_LOG_INFO;

	switch(level) {
		case LogLevel_Debug:	priority = ANDROID_LOG_DEBUG;	break;
		case LogLevel_Info:		priority = ANDROID_LOG_INFO;	break;
		case LogLevel_Warning:	priority = ANDROID_LOG_WARN;	break;
		case LogLevel_Error:	priority = ANDROID_LOG_ERROR;	break;
	}

	return __android_log_print(priority, tag, message);
}


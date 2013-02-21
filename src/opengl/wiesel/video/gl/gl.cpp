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
#include "gl.h"
#include <wiesel/util/log.h>


void wiesel::video::gl::checkGlError(const char *file, int line) {
    for (GLint error=glGetError(); error; error=glGetError()) {
    	const char *message = NULL;

    	switch(error) {
			case GL_INVALID_ENUM:			message = "GL_INVALID_ENUM";			break;
			case GL_INVALID_VALUE:			message = "GL_INVALID_VALUE";			break;
			case GL_INVALID_OPERATION:		message = "GL_INVALID_OPERATION";		break;
			case GL_OUT_OF_MEMORY:			message = "GL_OUT_OF_MEMORY";			break;
			default:						message = "unknown";					break;
    	}

        logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "@%s, %d: %s (0x%x)\n", file, line, message, error);
    }

    return;
}



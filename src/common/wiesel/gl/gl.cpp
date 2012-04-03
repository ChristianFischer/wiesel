/*
 * gl_utils.cpp
 *
 *  Created on: 02.04.2012
 *      Author: Baldur
 */

#include "gl.h"

#include <wiesel/platform/android/log.h>


void wiesel::checkGlError(const char *file, int line) {
    for (GLint error=glGetError(); error; error=glGetError()) {
    	const char *message = NULL;

    	switch(error) {
			case GL_INVALID_ENUM:			message = "GL_INVALID_ENUM";			break;
			case GL_INVALID_VALUE:			message = "GL_INVALID_VALUE";			break;
			case GL_INVALID_OPERATION:		message = "GL_INVALID_OPERATION";		break;
			case GL_OUT_OF_MEMORY:			message = "GL_OUT_OF_MEMORY";			break;
			default:						message = "unknown";					break;
    	}

        LOGE("GLES Error @%s, %d: %s (0x%x)\n", file, line, message, error);
    }

    return;
}



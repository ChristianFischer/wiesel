/*
 * gl_utils.h
 *
 *  Created on: 02.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_GL_GLUTILS_H__
#define __WIESEL_GL_GLUTILS_H__

// include OpenGL ES2 headers
#include <EGL/egl.h>
#include <GLES2/gl2.h>



#define CHECK_GL_ERROR		wiesel::checkGlError(__FILE__,__LINE__)


namespace wiesel
{
	/**
	 * @brief check for OpenGL errors and print the error message.
	 * @param file	The current source code file.
	 * @param line	The current source code line.
	 */
	void checkGlError(const char *file, int line);
}


#endif /* __WIESEL_GL_GLUTILS_H__ */

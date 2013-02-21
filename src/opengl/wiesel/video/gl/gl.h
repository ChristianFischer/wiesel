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
#ifndef __WIESEL_GL_GLUTILS_H__
#define __WIESEL_GL_GLUTILS_H__

#include <wiesel/wiesel-opengl.def>

// include platform specific OpenGL headers
#include "gl_import.h"

#define WIESEL_GL_LOG_TAG	"GL"
#define CHECK_GL_ERROR		wiesel::video::gl::checkGlError(__FILE__,__LINE__)


namespace wiesel {
namespace video {
namespace gl {

	/**
	 * @brief check for OpenGL errors and print the error message.
	 * @param file	The current source code file.
	 * @param line	The current source code line.
	 */
	WIESEL_OPENGL_EXPORT void checkGlError(const char *file, int line);

}
}
}


#endif /* __WIESEL_GL_GLUTILS_H__ */

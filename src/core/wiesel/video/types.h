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
#ifndef __WIESEL_VIDEO_TYPES_H__
#define	__WIESEL_VIDEO_TYPES_H__

#include <wiesel.h>
#include <wiesel/wiesel-core.def>

namespace wiesel {
	namespace video {

		/**
		 * @brief A list of possible primiteves at rendering.
		 */
		enum Primitive {
			Triangles,
			TriangleStrip,
			TriangleFan,
		};



		/**
		 * @brief A list of valid types for various graphics operations.
		 */
		enum ValueType {
			TypeInt32,

			TypeFloat,

			TypeVector2f,
			TypeVector3f,
			TypeVector4f,

			TypeMatrix4x4f,
		};


		/**
		 * @brief Determine a type's size in bytes.
		 */
		size_t WIESEL_CORE_EXPORT getTypeSize(ValueType type);
	}
}

#endif	/* __WIESEL_VIDEO_TYPES_H__ */

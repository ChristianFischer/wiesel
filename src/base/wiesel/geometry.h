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
#ifndef __WIESEL_GEOMETRY_H__
#define __WIESEL_GEOMETRY_H__


namespace wiesel {

	struct dimension
	{
	// constructors
	public:
		/// constructs a new dimension object with zero width and height
		dimension();
		
		/// constructs a new dimension object with width and height
		dimension(float width, float height);
		
		/// copy-constructor
		dimension(const dimension &other);
		
	// operations
	public:
		/// scale width and height with factor \c s
		void scale(float s);
		
		/// scale width and height with separate factors
		void scale(float sx, float sy);

	// members
	public:
		float width;
		float height;
	};
	
	
	inline dimension operator*(const dimension &dim, float s) {
		return dimension(dim.width * s, dim.height * s);
	}
	
	inline dimension& operator*=(dimension &dim, float s) {
		dim.width  *= s;
		dim.height *= s;
		return dim;
	}

}

#endif

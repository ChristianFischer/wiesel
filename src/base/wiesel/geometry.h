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


#include "math/vector2d.h"


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
		/// get the smallest dimension, either width or height.
		float getMin() const;

		/// get the largest dimension, either width or height.
		float getMax() const;

		/// get the average of width and height.
		float getAverage() const;

		/// get the ratio between width and height.
		float getRatio() const;

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



	/**
	 * @brief A basic class covering the boundings of a rectangle.
	 */
	class rect
	{
	public:
		/// creates a new rectangle with zero position and size.
		rect();

		/// creates a new rectangle with single values for position and size.
		rect(float x, float y, float w, float h);

		/// creates a new rectangle with a given position and size.
		rect(const vector2d &position, const dimension &size);

		/// creates a new rectangle with width and height and zero position.
		rect(float with, float height);

		/// creates a new rectangle with a given size and zero position.
		rect(const dimension &size);

		/// deconstructor
		~rect();

	public:
		vector2d		position;		//!< Position of the rect.
		dimension		size;			//!< Size of the rect.
	};

}

#endif

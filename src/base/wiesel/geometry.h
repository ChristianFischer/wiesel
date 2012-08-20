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

#include <ostream>


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
	class rectangle
	{
	public:
		/// creates a new rectangle with zero position and size.
		rectangle();

		/// creates a new rectangle with single values for position and size.
		rectangle(float x, float y, float w, float h);

		/// creates a new rectangle with a given position and size.
		rectangle(const vector2d &position, const dimension &size);

		/// creates a new rectangle with width and height and zero position.
		rectangle(float with, float height);

		/// creates a new rectangle with a given size and zero position.
		rectangle(const dimension &size);

		/// deconstructor
		~rectangle();

	// normalization
	public:
		/// normalize this rectangle, so with and height are always positive
		void normalize();

		/// get the normalized version of this rectangle. Keeps the original rectangle unchanged.
		rectangle normalized() const;

	// getters
	public:
		/// get the smallest x-position
		float getMinX() const;

		/// get the greatest x-position
		float getMaxX() const;

		/// get the centre x-position
		float getCenterX() const;

		/// get the smallest y-position
		float getMinY() const;

		/// get the greatest y-position
		float getMaxY() const;

		/// get the centre y-position
		float getCenterY() const;

	// tests
	public:
		/// tests, if this rectangle contains a specific point.
		bool contains(float x, float y) const;

		/// tests, if this rectangle contains a specific point.
		bool contains(const vector2d &v) const;

		/// tests, if this rectangle fully contains an other rectangle.
		bool contains(const rectangle &r) const;

		/// tests, if this rectangle intersects another rectangle
		bool intersects(const rectangle &r) const;

	public:
		vector2d		position;		//!< Position of the rect.
		dimension		size;			//!< Size of the rect.
	};



	/**
	 * @brief Create a new rectangle, which contains both parameter rectangles.
	 */
	rectangle createUnion(const rectangle &a, const rectangle &b);


	/**
	 * @brief Create the intersection of two rectangles.
	 */
	rectangle createIntersection(const rectangle &a, const rectangle &b);



	std::ostream& operator <<(std::ostream &o, const dimension &dim);
	std::ostream& operator <<(std::ostream &o, const rectangle &r);
	
}

#endif

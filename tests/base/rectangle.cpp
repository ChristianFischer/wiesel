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
#include "gtest/gtest.h"
#include "test_utils.h"

#include <wiesel/geometry.h>


using namespace wiesel;



// some predefined rectangle objects used to testing
static const float Ax =  -8;
static const float Ay = -13;
static const float Aw =  17;
static const float Ah =  39;
static const rectangle A    ( Ax,  Ay,  Aw,  Ah);



/**
 * Checks if the rectangle class has the exact size we expect -> 4 * sizeof(float)
 * When this test fails, there's maybe some hidden data, like a unwanted vtable.
 */
TEST(Rectangle, Sizeof) {
	EXPECT_EQ((4 * sizeof(float)), sizeof(rectangle));
}


/**
 * Checks the comparision operators
 */
TEST(Rectangle, Comparision) {
	rectangle r1(0, 0, 1, 1);
	rectangle r2(0, 0, 1, 1);

	EXPECT_TRUE (r1 == r2);
	EXPECT_FALSE(r1 != r2);
	EXPECT_RECT_EQ(r1, r2);
	EXPECT_RECT_EQ(rectangle(10, 20, 30, 40), rectangle(10, 20, 30, 40));

	// various tests, only one component different
	EXPECT_NE(rectangle( 0, -13, 17, 39), rectangle(-8, -13, 17, 39));
	EXPECT_NE(rectangle(-8,   0, 17, 39), rectangle(-8, -13, 17, 39));
	EXPECT_NE(rectangle(-8, -13,  7, 39), rectangle(-8, -13, 17, 39));
	EXPECT_NE(rectangle(-8, -13, 17,  9), rectangle(-8, -13, 17, 39));

	// various tests, only one component different
	EXPECT_FALSE(rectangle( 0, -13, 17, 39) == rectangle(-8, -13, 17, 39));
	EXPECT_FALSE(rectangle(-8,   0, 17, 39) == rectangle(-8, -13, 17, 39));
	EXPECT_FALSE(rectangle(-8, -13,  7, 39) == rectangle(-8, -13, 17, 39));
	EXPECT_FALSE(rectangle(-8, -13, 17,  9) == rectangle(-8, -13, 17, 39));
	EXPECT_FALSE(rectangle(-8, -13, 17, 39) == rectangle( 0, -13, 17, 39));
	EXPECT_FALSE(rectangle(-8, -13, 17, 39) == rectangle(-8,   0, 17, 39));
	EXPECT_FALSE(rectangle(-8, -13, 17, 39) == rectangle(-8, -13,  7, 39));
	EXPECT_FALSE(rectangle(-8, -13, 17, 39) == rectangle(-8, -13, 17,  9));

	EXPECT_TRUE (rectangle( 0, -13, 17, 39) != rectangle(-8, -13, 17, 39));
	EXPECT_TRUE (rectangle(-8,   0, 17, 39) != rectangle(-8, -13, 17, 39));
	EXPECT_TRUE (rectangle(-8, -13,  7, 39) != rectangle(-8, -13, 17, 39));
	EXPECT_TRUE (rectangle(-8, -13, 17,  9) != rectangle(-8, -13, 17, 39));
	EXPECT_TRUE (rectangle(-8, -13, 17, 39) != rectangle( 0, -13, 17, 39));
	EXPECT_TRUE (rectangle(-8, -13, 17, 39) != rectangle(-8,   0, 17, 39));
	EXPECT_TRUE (rectangle(-8, -13, 17, 39) != rectangle(-8, -13,  7, 39));
	EXPECT_TRUE (rectangle(-8, -13, 17, 39) != rectangle(-8, -13, 17,  9));
}


/**
 * Test creating a new rectangle.
 */
TEST(Rectangle, Create) {
	rectangle r1(Ax, Ay, Aw, Ah);
	EXPECT_EQ(Ax, r1.position.x);
	EXPECT_EQ(Ay, r1.position.y);
	EXPECT_EQ(Aw, r1.size.width);
	EXPECT_EQ(Ah, r1.size.height);

	rectangle r2(vector2d(Ax, Ay), dimension(Aw, Ah));
	EXPECT_EQ(Ax, r2.position.x);
	EXPECT_EQ(Ay, r2.position.y);
	EXPECT_EQ(Aw, r2.size.width);
	EXPECT_EQ(Ah, r2.size.height);

	EXPECT_EQ(r1, r2);
}


/**
 * Checks copying of a rectangle using copy constructor.
 */
TEST(Rectangle, CopyConstructor) {
	// prepare first rectangle
	rectangle r1(Ax, Ay, Aw, Ah);

	// create second rectangle as a copy of the first one
	rectangle r2(r1);

	// both rectangles should be equal
	EXPECT_RECT_EQ(r1, r2);
	EXPECT_EQ(r1, r2);
}


/**
 * Checks assigning a rectangle to another.
 */
TEST(Rectangle, Assign) {
	// prepare first rectangle
	rectangle r1(Ax, Ay, Aw, Ah);

	// create second rectangle
	rectangle r2;

	// v1 and v2 should not be eqal
	EXPECT_RECT_NE(r1, r2);

	// assign the first rectangle to the second one
	r2 = r1;

	// and both should be equal
	EXPECT_RECT_EQ(r1, r2);
	EXPECT_EQ(r1, r2);
}


/**
 * Test rectangle properties
 */
TEST(Rectangle, Properties) {
	EXPECT_VECTOR_EQ   (vector2d(-8, -13), A.position);
	EXPECT_DIMENSION_EQ(dimension(17, 39), A.size);

	EXPECT_EQ( -8,   A.getMinX());
	EXPECT_EQ(  9,   A.getMaxX());
	EXPECT_EQ(  0.5, A.getCenterX());
	EXPECT_EQ(-13,   A.getMinY());
	EXPECT_EQ( 26,   A.getMaxY());
	EXPECT_EQ(  6.5, A.getCenterY());
}


/**
 * Test normalization of rectangles.
 */
TEST(Rectangle, Normalization) {
	rectangle rect(25, 35, -10, -15);

	// this rect should not be normalized
	EXPECT_FALSE(rect.isNormalized());

	// try to get the normalized version of this rect
	rectangle n_rect = rect.normalized();

	EXPECT_TRUE(n_rect.isNormalized());
	EXPECT_EQ(rectangle(15, 20, 10, 15), n_rect);

	// the original should still be not normalized
	EXPECT_FALSE(rect.isNormalized());

	// now we try to normalize the original
	rect.normalize();

	EXPECT_TRUE(rect.isNormalized());

	// this rect should now be equal to the previously normalized version
	EXPECT_EQ(n_rect, rect);
}


/**
 * Test if a rectangle contains various points.
 */
TEST(Rectangle, ContainsPoint) {
	rectangle rect(-2, -1, 3, 3);

	// all these points should be within the rect
	EXPECT_TRUE(rect.contains( 0,  0));
	EXPECT_TRUE(rect.contains(-2, -1));
	EXPECT_TRUE(rect.contains( 1,  2));
	EXPECT_TRUE(rect.contains( 1, -1));
	EXPECT_TRUE(rect.contains(-2,  2));

	// these points should NOT be within the rect
	EXPECT_FALSE(rect.contains(-3, -1));
	EXPECT_FALSE(rect.contains(-2, -2));
	EXPECT_FALSE(rect.contains( 2,  2));
	EXPECT_FALSE(rect.contains( 1,  3));
	EXPECT_FALSE(rect.contains( 2, -1));
	EXPECT_FALSE(rect.contains( 1, -2));
	EXPECT_FALSE(rect.contains(-3,  2));
	EXPECT_FALSE(rect.contains(-2,  3));
}


/**
 * Test if a rectangle contains various points.
 */
TEST(Rectangle, ContainsVector) {
	rectangle rect(-2, -1, 3, 3);

	// all these points should be within the rect
	EXPECT_TRUE (rect.contains(vector2d( 0,  0)));
	EXPECT_TRUE (rect.contains(vector2d(-2, -1)));
	EXPECT_TRUE (rect.contains(vector2d( 1,  2)));
	EXPECT_TRUE (rect.contains(vector2d( 1, -1)));
	EXPECT_TRUE (rect.contains(vector2d(-2,  2)));

	// these points should NOT be within the rect
	EXPECT_FALSE(rect.contains(vector2d(-3, -1)));
	EXPECT_FALSE(rect.contains(vector2d(-2, -2)));
	EXPECT_FALSE(rect.contains(vector2d( 2,  2)));
	EXPECT_FALSE(rect.contains(vector2d( 1,  3)));
	EXPECT_FALSE(rect.contains(vector2d( 2, -1)));
	EXPECT_FALSE(rect.contains(vector2d( 1, -2)));
	EXPECT_FALSE(rect.contains(vector2d(-3,  2)));
	EXPECT_FALSE(rect.contains(vector2d(-2,  3)));
}


/**
 * Test if a rectangle contains another rectangle.
 */
TEST(Rectangle, ContainsRect) {
	rectangle rect(-2, -1, 3, 3);

	// rect contains itself
	EXPECT_TRUE (rect.contains(rect));

	// rect contains partial rectangles
	EXPECT_TRUE (rect.contains(rectangle(-2, -1, 1, 1)));
	EXPECT_TRUE (rect.contains(rectangle( 0,  1, 1, 1)));

	// rect does not contain intersecting rectangles
	EXPECT_FALSE(rect.contains(rectangle(-3, -1, 2, 2)));
	EXPECT_FALSE(rect.contains(rectangle(-2, -2, 2, 2)));
	EXPECT_FALSE(rect.contains(rectangle( 0,  1, 2, 2)));
	EXPECT_FALSE(rect.contains(rectangle(-3,  1, 2, 2)));
	EXPECT_FALSE(rect.contains(rectangle( 0, -2, 2, 2)));

	// rect does not contain rectangles completely outside
	EXPECT_FALSE(rect.contains(rectangle(-9, -9, 3, 3)));
	EXPECT_FALSE(rect.contains(rectangle( 9,  9, 3, 3)));
	EXPECT_FALSE(rect.contains(rectangle( 9, -9, 3, 3)));
	EXPECT_FALSE(rect.contains(rectangle(-9,  9, 3, 3)));
}


/**
 * Test if two rectangles intersects each other.
 */
TEST(Rectangle, Intersects) {
	rectangle rect(-2, -1, 3, 3);

	// rect intersects itself
	EXPECT_TRUE (rect.intersects(rect));

	// rect intersects partial rectangles
	EXPECT_TRUE (rect.intersects(rectangle(-2, -1, 1, 1)));
	EXPECT_TRUE (rect.intersects(rectangle( 0,  1, 1, 1)));

	// rect intersects partial intersections
	EXPECT_TRUE (rect.intersects(rectangle(-3, -1, 2, 2)));
	EXPECT_TRUE (rect.intersects(rectangle(-2, -2, 2, 2)));
	EXPECT_TRUE (rect.intersects(rectangle( 0,  1, 2, 2)));
	EXPECT_TRUE (rect.intersects(rectangle(-3,  1, 2, 2)));
	EXPECT_TRUE (rect.intersects(rectangle( 0, -2, 2, 2)));

	// rect does not intersect rectangles completely outside
	EXPECT_FALSE(rect.intersects(rectangle(-9, -9, 3, 3)));
	EXPECT_FALSE(rect.intersects(rectangle( 9,  9, 3, 3)));
	EXPECT_FALSE(rect.intersects(rectangle( 9, -9, 3, 3)));
	EXPECT_FALSE(rect.intersects(rectangle(-9,  9, 3, 3)));
}


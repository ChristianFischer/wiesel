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



// some predefined dimension objects used to testing
static const float Aw = 17;
static const float Ah = 39;
static const dimension A    ( Aw,  Ah);



/**
 * Checks if the dimension class has the exact size we expect -> 2 * sizeof(float)
 * When this test fails, there's maybe some hidden data, like a unwanted vtable.
 */
TEST(Dimension, Sizeof) {
	EXPECT_EQ((2 * sizeof(float)), sizeof(dimension));
}


/**
 * Checks the comparision operators
 */
TEST(Dimension, Comparision) {
	dimension dim1(0, 0);
	dimension dim2(0, 0);

	EXPECT_TRUE (dim1 == dim2);
	EXPECT_FALSE(dim1 != dim2);
	EXPECT_DIMENSION_EQ(dim1, dim2);
	EXPECT_DIMENSION_EQ(dimension(17, 39), dimension(17, 39));

	// various tests, only one component different
	EXPECT_NE(dimension(7, 39), dimension(17, 39));
	EXPECT_NE(dimension(17, 9), dimension(17, 39));

	// various tests, only one component different
	EXPECT_FALSE(dimension( 7, 39) == dimension(17, 39));
	EXPECT_FALSE(dimension(17,  9) == dimension(17, 39));
	EXPECT_FALSE(dimension(17, 39) == dimension( 7, 39));
	EXPECT_FALSE(dimension(17, 39) == dimension(17,  9));

	EXPECT_TRUE (dimension( 7, 39) != dimension(17, 39));
	EXPECT_TRUE (dimension(17,  9) != dimension(17, 39));
	EXPECT_TRUE (dimension(17, 39) != dimension( 7, 39));
	EXPECT_TRUE (dimension(17, 39) != dimension(17,  9));
}


/**
 * Test creating a new dimension.
 */
TEST(Dimension, Create) {
	dimension dim(Aw, Ah);
	EXPECT_EQ(Aw, dim.width);
	EXPECT_EQ(Ah, dim.height);
}


/**
 * Checks copying of a dimension using copy constructor.
 */
TEST(Dimension, CopyConstructor) {
	// prepare first dimension
	dimension dim1(Aw, Ah);

	// create second dimension as a copy of the first one
	dimension dim2(dim1);

	// both dimensions should be equal
	EXPECT_DIMENSION_EQ(dim1, dim2);
	EXPECT_EQ(dim1, dim2);
}


/**
 * Checks assigning a dimension object to another.
 */
TEST(Dimension, Assign) {
	// prepare first dimension
	dimension dim1(10, 20);

	// create second dimension
	dimension dim2;

	// v1 and v2 should not be eqal
	EXPECT_DIMENSION_NE(dim1, dim2);

	// assign the first dimension to the second one
	dim2 = dim1;

	// and both should be equal
	EXPECT_DIMENSION_EQ(dim1, dim2);
	EXPECT_EQ(dim1, dim2);
}


/**
 * Test Properties of a dimension object.
 */
TEST(Dimension, Properties) {
	dimension dim1(480, 320);
	EXPECT_EQ(320,   dim1.getMin());
	EXPECT_EQ(480,   dim1.getMax());
	EXPECT_EQ(400,   dim1.getAverage());
	EXPECT_FLOAT_EQ(1.5,   dim1.getRatio());

	dimension dim2(320, 480);
	EXPECT_EQ(320,   dim2.getMin());
	EXPECT_EQ(480,   dim2.getMax());
	EXPECT_EQ(400,   dim2.getAverage());
	EXPECT_FLOAT_EQ(1/1.5, dim2.getRatio());
}


/**
 * Test scale function.
 */
TEST(Dimension, Scale) {
	dimension dim1(100, 100);
	dim1.scale(5);
	EXPECT_DIMENSION_EQ(dimension(500, 500), dim1);

	dimension dim2(100, 100);
	dim2.scale(1, 5);
	EXPECT_DIMENSION_EQ(dimension(100, 500), dim2);

	dimension dim3(100, 100);
	dim3.scale(5, 1);
	EXPECT_DIMENSION_EQ(dimension(500, 100), dim3);
}





/**
 * Operators
 */
TEST(Dimension, OpMul) {
	EXPECT_DIMENSION_EQ(dimension(34, 78), (A * 2));
}

TEST(Dimension, OpMulNegative) {
	EXPECT_DIMENSION_EQ(dimension(-34, -78), (A * -2));
}

TEST(Dimension, OpMulAssign) {
	dimension dim = A;

	EXPECT_DIMENSION_EQ(dimension(34, 78), dim *= 2);		// return value should contain the correct result
	EXPECT_DIMENSION_EQ(dimension(34, 78), dim);			// dim should contain the correct result after processing
}

TEST(Dimension, OpMulAssignNegative) {
	dimension dim = A;

	EXPECT_DIMENSION_EQ(dimension(-34, -78), dim *= -2);	// return value should contain the correct result
	EXPECT_DIMENSION_EQ(dimension(-34, -78), dim);			// v should contain the correct result after processing
}


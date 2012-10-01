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

#include <wiesel/math/matrix.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/utils.h>


using namespace wiesel;



// some predefined vectors used to testing
static const float Ax =  7;
static const float Ay =  4;

static const float Bx =  2;
static const float By =  1;

static const float Cx =  2;
static const float Cy =  4;

static const vector2d A    ( Ax,  Ay);
static const vector2d A_NEG(-Ax, -Ay);
static const vector2d B    ( Bx,  By);
static const vector2d B_NEG(-Bx, -By);
static const vector2d C    ( Cx,  Cy);
static const vector2d C_NEG(-Cx, -Cy);



/**
 * Checks if the vector class has the exact size we expect -> 2 * sizeof(float)
 * When this test fails, there's maybe some hidden data, like a unwanted vtable.
 */
TEST(Vector2D, Sizeof) {
	EXPECT_EQ((2 * sizeof(float)), sizeof(vector2d));
}


/**
 * Checks the comparision operators
 */
TEST(Vector2D, Comparision) {
	EXPECT_TRUE (vector2d::zero == vector2d::zero);
	EXPECT_FALSE(vector2d::zero != vector2d::zero);
	EXPECT_VECTOR_EQ(vector2d::zero, vector2d::zero);
	EXPECT_VECTOR_EQ(vector2d(17, -5), vector2d(17, -5));

	// various tests, only one component different
	EXPECT_NE(vector2d(7, -5), vector2d(17, -5));
	EXPECT_NE(vector2d(17, 5), vector2d(17, -5));

	// various tests, only one component different
	EXPECT_FALSE(vector2d( 7, -5) == vector2d(17, -5));
	EXPECT_FALSE(vector2d(17,  5) == vector2d(17, -5));
	EXPECT_FALSE(vector2d(17, -5) == vector2d( 7, -5));
	EXPECT_FALSE(vector2d(17, -5) == vector2d(17,  5));

	EXPECT_TRUE (vector2d(17, -5) != vector2d( 7, -5));
	EXPECT_TRUE (vector2d(17, -5) != vector2d(17,  5));
	EXPECT_TRUE (vector2d( 7, -5) != vector2d(17, -5));
	EXPECT_TRUE (vector2d(17,  5) != vector2d(17, -5));
}


/**
 * Test creating a new vector.
 */
TEST(Vector2D, Create) {
	vector2d v(Ax, Ay);
	EXPECT_EQ(Ax, v.x);
	EXPECT_EQ(Ay, v.y);
}


/**
 * Checks copying of a vector using copy constructor.
 */
TEST(Vector2D, CopyConstructor) {
	// prepare first vector
	vector2d v1(10, 20);

	// create second vector as a copy of the first one
	vector2d v2(v1);

	// both vectors should be equal
	EXPECT_VECTOR_EQ(v1, v2);
	EXPECT_EQ(v1, v2);
}


/**
 * Checks assigning a vector to another.
 */
TEST(Vector2D, Assign) {
	// prepare first vector
	vector2d v1(10, 20);

	// create second vector
	vector2d v2 = vector2d::zero;

	// v1 and v2 should not be eqal
	EXPECT_VECTOR_NE(v1, v2);

	// assign the first vector to the second one
	v2 = v1;

	// and both should be equal
	EXPECT_VECTOR_EQ(v1, v2);
	EXPECT_EQ(v1, v2);
}





/**
 * Operators
 */
TEST(Vector2D, OpAdd) {
	EXPECT_VECTOR_EQ(vector2d(9, 5), (A + B));
}

TEST(Vector2D, OpAddNegative) {
	EXPECT_VECTOR_EQ(vector2d(5, 3), (A + B_NEG));
}

TEST(Vector2D, OpAddAssign) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(9, 5), v += B);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(9, 5), v);			// v should contain the correct result after processing
}

TEST(Vector2D, OpAddAssignNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(5, 3), v += B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(5, 3), v);			// v should contain the correct result after processing
}



TEST(Vector2D, OpSub) {
	EXPECT_VECTOR_EQ(vector2d(5, 3), (A - B));
}

TEST(Vector2D, OpSubNegative) {
	EXPECT_VECTOR_EQ(vector2d(9, 5), (A - B_NEG));
}

TEST(Vector2D, OpSubAssign) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(5, 3), v -= B);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(5, 3), v);			// v should contain the correct result after processing
}

TEST(Vector2D, OpSubAssignNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(9, 5), v -= B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(9, 5), v);			// v should contain the correct result after processing
}



TEST(Vector2D, OpMul) {
	EXPECT_VECTOR_EQ(vector2d(14, 4), (A * B));
}

TEST(Vector2D, OpMulNegative) {
	EXPECT_VECTOR_EQ(vector2d(-14, -4), (A * B_NEG));
}

TEST(Vector2D, OpMulAssign) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(14, 4), v *= B);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(14, 4), v);				// v should contain the correct result after processing
}

TEST(Vector2D, OpMulAssignNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(-14, -4), v *= B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(-14, -4), v);				// v should contain the correct result after processing
}



TEST(Vector2D, OpMulScalar) {
	EXPECT_VECTOR_EQ(vector2d(35, 20), (A * 5));
}

TEST(Vector2D, OpMulScalarNegative) {
	EXPECT_VECTOR_EQ(vector2d(-35, -20), (A * -5));
}

TEST(Vector2D, OpMulAssignScalar) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(35, 20), v *= 5);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(35, 20), v);				// v should contain the correct result after processing
}

TEST(Vector2D, OpMulAssignScalarNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(-35, -20), v *= -5);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(-35, -20), v);			// v should contain the correct result after processing
}



TEST(Vector2D, OpDiv) {
	EXPECT_VECTOR_EQ(vector2d(3.5f, 1.0f), (A / C));
}

TEST(Vector2D, OpDivNegative) {
	EXPECT_VECTOR_EQ(vector2d(-3.5f, -1.0f), (A / C_NEG));
}

TEST(Vector2D, OpDivAssign) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(3.5f, 1.0f), v /= C);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(3.5f, 1.0f), v);				// v should contain the correct result after processing
}

TEST(Vector2D, OpDivAssignNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(-3.5f, -1.0f), v /= C_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(-3.5f, -1.0f), v);			// v should contain the correct result after processing
}



TEST(Vector2D, OpDivScalar) {
	EXPECT_VECTOR_EQ(vector2d(3.5f, 2.0f), (A / 2));
}

TEST(Vector2D, OpDivScalarNegative) {
	EXPECT_VECTOR_EQ(vector2d(-3.5f, -2.0f), (A / -2));
}

TEST(Vector2D, OpDivAssignScalar) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(3.5f, 2.0f), v /= 2);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(3.5f, 2.0f), v);				// v should contain the correct result after processing
}

TEST(Vector2D, OpDivAssignScalarNegative) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(vector2d(-3.5f, -2.0f), v /= -2);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector2d(-3.5f, -2.0f), v);			// v should contain the correct result after processing
}



TEST(Vector2D, OpMulIdentity) {
	EXPECT_VECTOR_EQ(A, A * matrix4x4::identity);
}

TEST(Vector2D, OpMulAssignIdentity) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(A, v *= matrix4x4::identity);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(A, v);									// v should contain the correct result after processing
}



TEST(Vector2D, OpDivIdentity) {
	EXPECT_VECTOR_EQ(A, A / matrix4x4::identity);
}

TEST(Vector2D, OpDivAssignIdentity) {
	vector2d v = A;

	EXPECT_VECTOR_EQ(A, v /= matrix4x4::identity);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(A, v);									// v should contain the correct result after processing
}



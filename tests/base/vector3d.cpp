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
#include <wiesel/math/vector3d.h>
#include <wiesel/math/utils.h>


using namespace wiesel;



// some predefined vectors used to testing
static const float Ax =  7;
static const float Ay =  4;
static const float Az =  9;

static const float Bx =  2;
static const float By =  1;
static const float Bz = 17;

static const float Cx =  2;
static const float Cy =  4;
static const float Cz = 18;

static const vector3d A    ( Ax,  Ay,  Az);
static const vector3d A_NEG(-Ax, -Ay, -Az);
static const vector3d B    ( Bx,  By,  Bz);
static const vector3d B_NEG(-Bx, -By, -Bz);
static const vector3d C    ( Cx,  Cy,  Cz);
static const vector3d C_NEG(-Cx, -Cy, -Cz);



/**
 * Checks if the vector class has the exact size we expect -> 3 * sizeof(float)
 * When this test fails, there's maybe some hidden data, like a unwanted vtable.
 */
TEST(Vector3D, Sizeof) {
	EXPECT_EQ((3 * sizeof(float)), sizeof(vector3d));
}


/**
 * Checks the comparision operators
 */
TEST(Vector3D, Comparision) {
	EXPECT_TRUE (vector3d::zero == vector3d::zero);
	EXPECT_FALSE(vector3d::zero != vector3d::zero);
	EXPECT_VECTOR_EQ(vector3d::zero, vector3d::zero);
	EXPECT_VECTOR_EQ(vector3d(17, -5, 9), vector3d(17, -5, 9));

	// various tests, only one component different
	EXPECT_NE(vector3d(7, -5,  9), vector3d(17, -5, 9));
	EXPECT_NE(vector3d(17, 5,  9), vector3d(17, -5, 9));
	EXPECT_NE(vector3d(17, -5, 1), vector3d(17, -5, 9));

	// various tests, only one component different
	EXPECT_FALSE(vector3d( 7, -5,  9) == vector3d(17, -5,  9));
	EXPECT_FALSE(vector3d(17,  5,  9) == vector3d(17, -5,  9));
	EXPECT_FALSE(vector3d(17, -5,  1) == vector3d(17, -5,  9));
	EXPECT_FALSE(vector3d(17, -5,  9) == vector3d( 7, -5,  9));
	EXPECT_FALSE(vector3d(17, -5,  9) == vector3d(17,  5,  9));
	EXPECT_FALSE(vector3d(17, -5,  9) == vector3d(17, -5,  1));

	EXPECT_TRUE (vector3d(17, -5,  9) != vector3d( 7, -5,  9));
	EXPECT_TRUE (vector3d(17, -5,  9) != vector3d(17,  5,  9));
	EXPECT_TRUE (vector3d(17, -5,  9) != vector3d(17, -5,  1));
	EXPECT_TRUE (vector3d( 7, -5,  9) != vector3d(17, -5,  9));
	EXPECT_TRUE (vector3d(17,  5,  9) != vector3d(17, -5,  9));
	EXPECT_TRUE (vector3d(17, -5,  1) != vector3d(17, -5,  9));
}


/**
 * Test creating a new vector.
 */
TEST(Vector3D, Create) {
	vector3d v(Ax, Ay, Az);
	EXPECT_EQ(Ax, v.x);
	EXPECT_EQ(Ay, v.y);
	EXPECT_EQ(Az, v.z);
}


/**
 * Checks copying of a vector using copy constructor.
 */
TEST(Vector3D, CopyConstructor) {
	// prepare first vector
	vector3d v1(10, 20, 30);

	// create second vector as a copy of the first one
	vector3d v2(v1);

	// both vectors should be equal
	EXPECT_VECTOR_EQ(v1, v2);
	EXPECT_EQ(v1, v2);
}


/**
 * Checks assigning a vector to another.
 */
TEST(Vector3D, Assign) {
	// prepare first vector
	vector3d v1(10, 20, 30);

	// create second vector
	vector3d v2 = vector3d::zero;

	// v1 and v2 should not be eqal
	EXPECT_VECTOR_NE(v1, v2);

	// assign the first vector to the second one
	v2 = v1;

	// and both should be equal
	EXPECT_VECTOR_EQ(v1, v2);
	EXPECT_EQ(v1, v2);
}


/**
 * Test the cross product of various normalized vectors.
 */
TEST(Vector3D, CrossProduct) {
	EXPECT_VECTOR_EQ(vector3d( 0,  0,  0), cross(vector3d( 0,  0,  0), vector3d( 0,  0,  0)));

	EXPECT_VECTOR_EQ(vector3d( 0,  0,  1), cross(vector3d( 1,  0,  0), vector3d( 0,  1,  0)));
	EXPECT_VECTOR_EQ(vector3d( 0,  0, -1), cross(vector3d( 0,  1,  0), vector3d( 1,  0,  0)));
	EXPECT_VECTOR_EQ(vector3d( 0,  0,  1), cross(vector3d(-1,  0,  0), vector3d( 0, -1,  0)));
	EXPECT_VECTOR_EQ(vector3d( 0,  0, -1), cross(vector3d( 0, -1,  0), vector3d(-1,  0,  0)));

	EXPECT_VECTOR_EQ(vector3d( 0, -1,  0), cross(vector3d( 1,  0,  0), vector3d( 0,  0,  1)));
	EXPECT_VECTOR_EQ(vector3d( 0,  1,  0), cross(vector3d( 0,  0,  1), vector3d( 1,  0,  0)));
	EXPECT_VECTOR_EQ(vector3d( 0, -1,  0), cross(vector3d(-1,  0,  0), vector3d( 0,  0, -1)));
	EXPECT_VECTOR_EQ(vector3d( 0,  1,  0), cross(vector3d( 0,  0, -1), vector3d(-1,  0,  0)));

	EXPECT_VECTOR_EQ(vector3d( 1,  0,  0), cross(vector3d( 0,  1,  0), vector3d( 0,  0,  1)));
	EXPECT_VECTOR_EQ(vector3d(-1,  0,  0), cross(vector3d( 0,  0,  1), vector3d( 0,  1,  0)));
	EXPECT_VECTOR_EQ(vector3d( 1,  0,  0), cross(vector3d( 0, -1,  0), vector3d( 0,  0, -1)));
	EXPECT_VECTOR_EQ(vector3d(-1,  0,  0), cross(vector3d( 0,  0, -1), vector3d( 0, -1,  0)));
}





/**
 * Operators
 */
TEST(Vector3D, OpAdd) {
	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), (A + B));
}

TEST(Vector3D, OpAddNegative) {
	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), (A + B_NEG));
}

TEST(Vector3D, OpAddAssign) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), v += B);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), v);			// v should contain the correct result after processing
}

TEST(Vector3D, OpAddAssignNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), v += B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), v);			// v should contain the correct result after processing
}



TEST(Vector3D, OpSub) {
	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), (A - B));
}

TEST(Vector3D, OpSubNegative) {
	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), (A - B_NEG));
}

TEST(Vector3D, OpSubAssign) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), v -= B);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(5, 3, -8), v);			// v should contain the correct result after processing
}

TEST(Vector3D, OpSubAssignNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), v -= B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(9, 5, 26), v);			// v should contain the correct result after processing
}



TEST(Vector3D, OpMul) {
	EXPECT_VECTOR_EQ(vector3d(14, 4, 153), (A * B));
}

TEST(Vector3D, OpMulNegative) {
	EXPECT_VECTOR_EQ(vector3d(-14, -4, -153), (A * B_NEG));
}

TEST(Vector3D, OpMulAssign) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(14, 4, 153), v *= B);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(14, 4, 153), v);				// v should contain the correct result after processing
}

TEST(Vector3D, OpMulAssignNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(-14, -4, -153), v *= B_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(-14, -4, -153), v);			// v should contain the correct result after processing
}



TEST(Vector3D, OpMulScalar) {
	EXPECT_VECTOR_EQ(vector3d(35, 20, 45), (A * 5));
}

TEST(Vector3D, OpMulScalarNegative) {
	EXPECT_VECTOR_EQ(vector3d(-35, -20, -45), (A * -5));
}

TEST(Vector3D, OpMulAssignScalar) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(35, 20, 45), v *= 5);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(35, 20, 45), v);				// v should contain the correct result after processing
}

TEST(Vector3D, OpMulAssignScalarNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(-35, -20, -45), v *= -5);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(-35, -20, -45), v);			// v should contain the correct result after processing
}



TEST(Vector3D, OpDiv) {
	EXPECT_VECTOR_EQ(vector3d(3.5f, 1.0f, 0.5f), (A / C));
}

TEST(Vector3D, OpDivNegative) {
	EXPECT_VECTOR_EQ(vector3d(-3.5f, -1.0f, -0.5f), (A / C_NEG));
}

TEST(Vector3D, OpDivAssign) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(3.5f, 1.0f, 0.5f), v /= C);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(3.5f, 1.0f, 0.5f), v);				// v should contain the correct result after processing
}

TEST(Vector3D, OpDivAssignNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(-3.5f, -1.0f, -0.5f), v /= C_NEG);	// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(-3.5f, -1.0f, -0.5f), v);				// v should contain the correct result after processing
}



TEST(Vector3D, OpDivScalar) {
	EXPECT_VECTOR_EQ(vector3d(3.5f, 2.0f, 4.5f), (A / 2));
}

TEST(Vector3D, OpDivScalarNegative) {
	EXPECT_VECTOR_EQ(vector3d(-3.5f, -2.0f, -4.5f), (A / -2));
}

TEST(Vector3D, OpDivAssignScalar) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(3.5f, 2.0f, 4.5f), v /= 2);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(3.5f, 2.0f, 4.5f), v);				// v should contain the correct result after processing
}

TEST(Vector3D, OpDivAssignScalarNegative) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(vector3d(-3.5f, -2.0f, -4.5f), v /= -2);		// return value should contain the correct result
	EXPECT_VECTOR_EQ(vector3d(-3.5f, -2.0f, -4.5f), v);				// v should contain the correct result after processing
}



TEST(Vector3D, OpMulIdentity) {
	EXPECT_VECTOR_EQ(A, A * matrix4x4::identity);
}

TEST(Vector3D, OpMulAssignIdentity) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(A, v *= matrix4x4::identity);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(A, v);									// v should contain the correct result after processing
}



TEST(Vector3D, OpDivIdentity) {
	EXPECT_VECTOR_EQ(A, A / matrix4x4::identity);
}

TEST(Vector3D, OpDivAssignIdentity) {
	vector3d v = A;

	EXPECT_VECTOR_EQ(A, v /= matrix4x4::identity);			// return value should contain the correct result
	EXPECT_VECTOR_EQ(A, v);									// v should contain the correct result after processing
}



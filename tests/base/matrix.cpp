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



/**
 * Checks if the matrix class has the exact size we expect -> 16 * sizeof(float)
 * When this test fails, there's maybe some hidden data, like a unwanted vtable.
 */
TEST(Matrix, Sizeof) {
	EXPECT_EQ((16 * sizeof(float)), sizeof(matrix4x4));
}


/**
 * Checks the comparision operators
 */
TEST(Matrix, Comparision) {
	EXPECT_TRUE (matrix4x4::identity == matrix4x4::identity);
	EXPECT_FALSE(matrix4x4::identity != matrix4x4::identity);
	EXPECT_MATRIX_EQ(matrix4x4::identity, matrix4x4::identity);
	EXPECT_MATRIX_IDENTITY(matrix4x4::identity);
}


/**
 * Checks copying of a matrix using copy constructor.
 */
TEST(Matrix, CopyConstructor) {
	// prepare first matrix
	matrix4x4 m1;
	for(int i=0; i<16; i++) {
		m1.m[i] = static_cast<float>(i);
	}

	// the prepared matrix should be different to the identity
	EXPECT_MATRIX_NOT_IDENTITY(m1);

	// create second matrix as a copy of the first one
	matrix4x4 m2(m1);

	// the second matrix should be different to the identity
	EXPECT_MATRIX_NOT_IDENTITY(m2);

	// and both should be equal
	EXPECT_MATRIX_EQ(m1, m2);
	EXPECT_EQ(m1, m2);
}


/**
 * Checks assigning a matrix to another.
 */
TEST(Matrix, Assign) {
	// prepare first matrix
	matrix4x4 m1;
	for(int i=0; i<16; i++) {
		m1.m[i] = static_cast<float>(i);
	}

	// the prepared matrix should be different to the identity
	EXPECT_MATRIX_NOT_IDENTITY(m1);

	// create second matrix
	matrix4x4 m2 = matrix4x4::identity;

	// this matrix should be the identity
	EXPECT_MATRIX_IDENTITY(m2);

	// m1 and m2 should not be eqal
	EXPECT_MATRIX_NE(m1, m2);

	// assign the first matrix to the second one
	m2 = m1;

	// now the second matrix should also be different to the identity
	EXPECT_MATRIX_NOT_IDENTITY(m2);

	// and both should be equal
	EXPECT_MATRIX_EQ(m1, m2);
	EXPECT_EQ(m1, m2);
}


/**
 * Test a simple multiplication: identity multiplied with itself should still be identity.
 */
TEST(Matrix, SimpleMultiplication) {
	EXPECT_MATRIX_IDENTITY(matrix4x4::identity * matrix4x4::identity);
}


/**
 * Test a simple multiplication: identity divided by itself should still be identity.
 */
TEST(Matrix, SimpleDivision) {
	EXPECT_MATRIX_IDENTITY(matrix4x4::identity / matrix4x4::identity);
}


/**
 * Test matrix determinant.
 */
TEST(Matrix, DeterminantOfIdentity) {
	EXPECT_FLOAT_EQ(1.0f, matrix4x4::identity.det());
}


/**
 * Checks, if the matrix can be inverted.
 */
TEST(Matrix, Invert) {
	matrix4x4 matrix = matrix4x4::identity;

	// apply some transformations to the matrix (the result is not important yet)
	matrix.scale(3, 4, 5);
	matrix.rotateX(45);
	matrix.rotateY(90);
	matrix.rotateZ(180);
	matrix.translate(1, 2, 3);

	// now the matrix should have changed
	EXPECT_MATRIX_NOT_IDENTITY(matrix);

	// compute the inverted matrix
	matrix4x4 inverted = matrix.inverted();

	// the result should be different from the original matrix, also it should NOT be the identity matrix
	EXPECT_MATRIX_NE(matrix,              inverted);
	EXPECT_MATRIX_NE(matrix4x4::identity, inverted);
	EXPECT_MATRIX_NOT_IDENTITY(inverted);

	// check, if the inversion of the inverted matrix matches the original
	EXPECT_MATRIX_EQ(matrix, inverted.inverted());

	// check, if (matrix * inverted) produces the identity
	EXPECT_MATRIX_IDENTITY(matrix * inverted);
}


/**
 * Tests translation.
 */
TEST(Matrix, TransformationTranslate) {
	matrix4x4 translated = matrix4x4::identity;
	translated.translate(1, 2, 3);

	// check if transformation was applied
	EXPECT_NE(matrix4x4::identity, translated);

	// check if transformation can be reverted
	EXPECT_EQ(matrix4x4::identity, translated / translated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_EQ(vector3d(1, 2, 3), vector3d::zero * translated);
}


/**
 * Tests translation.
 */
TEST(Matrix, TransformationTranslateNegative) {
	matrix4x4 translated = matrix4x4::identity;
	translated.translate(-1, -2, -3);

	// check if transformation was applied
	EXPECT_NE(matrix4x4::identity, translated);

	// check if transformation can be reverted
	EXPECT_EQ(matrix4x4::identity, translated / translated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_EQ(vector3d(-1, -2, -3), vector3d::zero * translated);
}


/**
 * Tests scaling.
 */
TEST(Matrix, TransformationScale) {
	matrix4x4 scaled = matrix4x4::identity;
	scaled.scale(2, 3, 4);

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(scaled);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(scaled / scaled);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(10, 45, 80), vector3d(5, 15, 20) * scaled);
}


/**
 * Tests negative scaling.
 */
TEST(Matrix, TransformationScaleNegative) {
	matrix4x4 scaled = matrix4x4::identity;
	scaled.scale(-2, -3, -4);

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(scaled);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(scaled / scaled);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(-10, -45, -80), vector3d(5, 15, 20) * scaled);
}


/**
 * Tests x rotation.
 */
TEST(Matrix, TransformationRotateX) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateX(deg2rad(90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, -10, 0), vector3d(0, 0, 10) * rotated);
}


/**
 * Tests negative x rotation.
 */
TEST(Matrix, TransformationRotateNegativeX) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateX(deg2rad(-90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, 10, 0), vector3d(0, 0, 10) * rotated);
}


/**
 * Tests y rotation.
 */
TEST(Matrix, TransformationRotateY) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateY(deg2rad(90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, 0, -10), vector3d(10, 0, 0) * rotated);
}


/**
 * Tests negative y rotation.
 */
TEST(Matrix, TransformationRotateNegativeY) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateY(deg2rad(-90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, 0, 10), vector3d(10, 0, 0) * rotated);
}


/**
 * Tests z rotation.
 */
TEST(Matrix, TransformationRotateZ) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateZ(deg2rad(90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, 10, 0), vector3d(10, 0, 0) * rotated);
}


/**
 * Tests negative z rotation.
 */
TEST(Matrix, TransformationRotateNegativeZ) {
	matrix4x4 rotated = matrix4x4::identity;
	rotated.rotateZ(deg2rad(-90));

	// check if transformation was applied
	EXPECT_MATRIX_NOT_IDENTITY(rotated);

	// check if transformation can be reverted
	EXPECT_MATRIX_IDENTITY(rotated / rotated);

	// check if a transformation applied to a vector produces the correct result
	EXPECT_VECTOR_EQ(vector3d(0, -10, 0), vector3d(10, 0, 0) * rotated);
}


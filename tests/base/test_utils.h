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
#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <wiesel/math/matrix.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>
#include <wiesel/geometry.h>

#include <gtest/gtest.h>
#include <cmath>


#ifndef UNITTEST_EPSILON
#define UNITTEST_EPSILON (0.000001)
#endif


// create googletest-like macros
#define EXPECT_DIMENSION_EQ(expected, actual)	EXPECT_PRED_FORMAT2(compareDimensionUsingEpsilon, expected, actual)
#define EXPECT_DIMENSION_NE(expected, actual)	EXPECT_FALSE(compareDimensionUsingEpsilon(expected, actual))
#define EXPECT_RECT_EQ(expected, actual)		EXPECT_PRED_FORMAT2(compareRectangleUsingEpsilon, expected, actual)
#define EXPECT_RECT_NE(expected, actual)		EXPECT_FALSE(compareRectangleUsingEpsilon(expected, actual))

#define EXPECT_VECTOR_EQ(expected, actual)		EXPECT_PRED_FORMAT2(compareVectorUsingEpsilon, expected, actual)
#define EXPECT_VECTOR_NE(expected, actual)		EXPECT_FALSE(compareVectorUsingEpsilon(expected, actual))
#define EXPECT_MATRIX_EQ(expected, actual)		EXPECT_PRED_FORMAT2(compareMatrixUsingEpsilon, expected, actual)
#define EXPECT_MATRIX_NE(expected, actual)		EXPECT_FALSE(compareMatrixUsingEpsilon(expected, actual))
#define EXPECT_MATRIX_IDENTITY(actual)			EXPECT_MATRIX_EQ(::wiesel::matrix4x4::identity, actual)
#define EXPECT_MATRIX_NOT_IDENTITY(actual)		EXPECT_MATRIX_NE(::wiesel::matrix4x4::identity, actual)



namespace wiesel {


	/**
	 * @brief compare two floats using epsilon
	 */
	template <typename FLOAT>
	bool compareFloatsUsingEpsilon(FLOAT a, FLOAT b, FLOAT epsilon=UNITTEST_EPSILON) {
		if (a == b) {
			return true;
		}

		FLOAT diff = std::abs(a - b);
		if (diff <= epsilon) {
			return true;
		}

		return false;
	}



	/**
	 * @brief helper function for comparing two dimensions using an epsilon value.
	 */
	bool compareDimensionUsingEpsilon(
							const ::wiesel::dimension &expected,
							const ::wiesel::dimension &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief helper function for comparing two rectangles using an epsilon value.
	 */
	bool compareRectangleUsingEpsilon(
							const ::wiesel::rectangle &expected,
							const ::wiesel::rectangle &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief helper function for comparing two vectors using an epsilon value.
	 */
	bool compareVectorUsingEpsilon(
							const ::wiesel::vector2d &expected,
							const ::wiesel::vector2d &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief helper function for comparing two vectors using an epsilon value.
	 */
	bool compareVectorUsingEpsilon(
							const ::wiesel::vector3d &expected,
							const ::wiesel::vector3d &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief helper function for comparing two matrices using an epsilon value.
	 */
	bool compareMatrixUsingEpsilon(
							const ::wiesel::matrix4x4 &expected,
							const ::wiesel::matrix4x4 &actual,
							float epsilon=UNITTEST_EPSILON
	);





	/**
	 * @brief GoogleTest helper function for comparing two dimensions.
	 */
	::testing::AssertionResult compareDimensionUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const ::wiesel::dimension &expected,
							const ::wiesel::dimension &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief GoogleTest helper function for comparing two rectangles.
	 */
	::testing::AssertionResult compareRectangleUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const ::wiesel::rectangle &expected,
							const ::wiesel::rectangle &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief GoogleTest helper function for comparing two vectors.
	 */
	::testing::AssertionResult compareVectorUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const ::wiesel::vector2d &expected,
							const ::wiesel::vector2d &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief GoogleTest helper function for comparing two vectors.
	 */
	::testing::AssertionResult compareVectorUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const ::wiesel::vector3d &expected,
							const ::wiesel::vector3d &actual,
							float epsilon=UNITTEST_EPSILON
	);


	/**
	 * @brief GoogleTest helper function for comparing two matrices.
	 */
	::testing::AssertionResult compareMatrixUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const ::wiesel::matrix4x4 &expected,
							const ::wiesel::matrix4x4 &actual,
							float epsilon=UNITTEST_EPSILON
	);

} // namespace wiesel

#endif // __TEST_UTILS_H__

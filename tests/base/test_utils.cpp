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
#include "test_utils.h"


namespace wiesel {

	bool compareVectorUsingEpsilon(
							const vector2d &expected,
							const vector2d &actual,
							float epsilon
	) {
		return
				compareFloatsUsingEpsilon(expected.x, actual.x, epsilon)
			&&	compareFloatsUsingEpsilon(expected.y, actual.y, epsilon)
		;
	}


	bool compareVectorUsingEpsilon(
							const vector3d &expected,
							const vector3d &actual,
							float epsilon
	) {
		return
				compareFloatsUsingEpsilon(expected.x, actual.x, epsilon)
			&&	compareFloatsUsingEpsilon(expected.y, actual.y, epsilon)
			&&	compareFloatsUsingEpsilon(expected.z, actual.z, epsilon)
		;
	}




	bool compareMatrixUsingEpsilon(
							const matrix4x4 &expected,
							const matrix4x4 &actual,
							float epsilon
	) {
		for(int i=16; --i>=0;) {
			if (compareFloatsUsingEpsilon(actual.m[i], expected.m[i], epsilon) == false) {
				return false;
			}
		}

		return true;
	}





	::testing::AssertionResult compareVectorUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const vector2d &expected,
							const vector2d &actual,
							float epsilon
	) {
		if (compareVectorUsingEpsilon(expected, actual) == false) {
			return ::testing::internal::EqFailure(
								expected_expression,
								actual_expression,
								::testing::internal::FormatForComparisonFailureMessage(expected, actual),
								::testing::internal::FormatForComparisonFailureMessage(actual, expected),
								false
			);
		}

		return ::testing::AssertionSuccess();
	}


	::testing::AssertionResult compareVectorUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const vector3d &expected,
							const vector3d &actual,
							float epsilon
	) {
		if (compareVectorUsingEpsilon(expected, actual) == false) {
			return ::testing::internal::EqFailure(
								expected_expression,
								actual_expression,
								::testing::internal::FormatForComparisonFailureMessage(expected, actual),
								::testing::internal::FormatForComparisonFailureMessage(actual, expected),
								false
			);
		}

		return ::testing::AssertionSuccess();
	}



	::testing::AssertionResult compareMatrixUsingEpsilon(
							const char* expected_expression,
							const char* actual_expression,
							const matrix4x4 &expected,
							const matrix4x4 &actual,
							float epsilon
	) {
		if (compareMatrixUsingEpsilon(actual, expected, epsilon) == false) {
			::std::stringstream ss_expected;
			::std::stringstream ss_actual;

			ss_expected << ::std::endl << expected << ::std::endl;
			ss_actual   << ::std::endl << actual   << ::std::endl;

			return ::testing::internal::EqFailure(expected_expression,
							 actual_expression,
							 ss_expected.str(),
							 ss_actual.str(),
							 false
			);
		}

		return ::testing::AssertionSuccess();
	}

} // namespace wiesel

#include "gtest/gtest.h"

#include <wiesel/math/matrix.h>

using namespace wiesel;



/**
 * Checks the comparision operators
 */
TEST(MatrixTest, Comparision) {
	EXPECT_TRUE (matrix4x4::identity == matrix4x4::identity);
	EXPECT_FALSE(matrix4x4::identity != matrix4x4::identity);
}

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

#include <wiesel/math/utils.h>
#include <cmath>

using namespace wiesel;



/**
 * Test degrees to radians conversion
 */
TEST(MathUtils, Degrees2Radians) {
	EXPECT_FLOAT_EQ(0.0f,   deg2rad(  0.0f));
	EXPECT_FLOAT_EQ(M_PI_4, deg2rad( 45.0f));
	EXPECT_FLOAT_EQ(M_PI_2, deg2rad( 90.0f));
	EXPECT_FLOAT_EQ(M_PI,   deg2rad(180.0f));
	EXPECT_FLOAT_EQ(M_PI*2, deg2rad(360.0f));
}

/**
 * Check if degrees to radians conversion is reversible.
 */
TEST(MathUtils, Degrees2RadiansReversible) {
	for(float deg=0.0f; deg<=720.0f; deg+=1.0f) {
		EXPECT_FLOAT_EQ(deg, rad2deg(deg2rad(deg)));
	}

	return;
}



/**
 * Test radians to degrees conversion
 */
TEST(MathUtils, Radians2Degrees) {
	EXPECT_FLOAT_EQ(  0.0f, rad2deg(0.0f));
	EXPECT_FLOAT_EQ( 45.0f, rad2deg(M_PI_4));
	EXPECT_FLOAT_EQ( 90.0f, rad2deg(M_PI_2));
	EXPECT_FLOAT_EQ(180.0f, rad2deg(M_PI));
	EXPECT_FLOAT_EQ(360.0f, rad2deg(M_PI*2));
}

/**
 * Check if radians to degrees conversion is reversible.
 */
TEST(MathUtils, Radians2DegreesReversible) {
	for(float rad=0.0f; rad<=720.0f; rad+=1.0f) {
		EXPECT_FLOAT_EQ(rad, deg2rad(rad2deg(rad)));
	}

	return;
}



/**
 * Checks the float comparision utility.
 */
TEST(MathUtils, AreFloatsAlmostEqual_Zero) {
	EXPECT_TRUE (areFloatsAlmostEqual(0.0f, 0.0f));
	EXPECT_TRUE (areFloatsAlmostEqual(0.0d, 0.0d));
}

/**
 * Checks the float comparision utility.
 */
TEST(MathUtils, AreFloatsAlmostEqual_One) {
	EXPECT_TRUE (areFloatsAlmostEqual(1.0f, 1.0f));
	EXPECT_TRUE (areFloatsAlmostEqual(1.0d, 1.0d));
}


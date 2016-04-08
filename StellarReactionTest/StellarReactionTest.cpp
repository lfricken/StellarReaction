#include "stdafx.h"
#include "gtest/gtest.h"
#include "Game.hpp"

Game game;

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);

	int returnValue = RUN_ALL_TESTS();

	std::cout << "\nTesting Done!\nPress enter to continue...\n";
	std::cin.get();

	return returnValue;
}
/**

USE THESE:

==
EXPECT_EQ(expected, actual);
!=
EXPECT_NE(val1, val2);
<
EXPECT_LT(val1, val2);
<=
EXPECT_LE(val1, val2);
>
EXPECT_GT(val1, val2);
>=
EXPECT_GE(val1, val2);


== true
EXPECT_TRUE(condition)
== false
EXPECT_FALSE(condition)


two float values are ALMOST equal
EXPECT_FLOAT_EQ(expected, actual)

two double values are ALMOST equal
EXPECT_DOUBLE_EQ(expected, actual)

two values near
EXPECT_NEAR(val1, val2, abs_err)


**/

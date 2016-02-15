// StellarReactionTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "gtest/gtest.h"

double square(double x){
	return (double)(x * x);
};

TEST(SquareRootTest, PositiveNos) {
	EXPECT_EQ(25.0, square(5.0));
}

TEST(SquareRootTest, ZeroAndNegativeNos) {
	EXPECT_EQ(9.0, square(2.0));
	ASSERT_EQ(9.0, square(3.0));


	ASSERT_EQ(10, square(4.0));

	ASSERT_EQ(11, square(5.0));

}
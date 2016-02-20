#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"

class StealthTest : public ::testing::Test
{
protected:
	virtual void setUp()
	{

	}

	virtual void tearDown()
	{

	}
};

TEST(StealthTest, stealthActive)
{
	EXPECT_EQ(1.0, 1.0);
}

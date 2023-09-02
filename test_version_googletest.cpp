#include "lib.h"

#include <gtest/gtest.h>

TEST(LibTest, test_valid_version) 
{
    EXPECT_TRUE(version() > 0);
}

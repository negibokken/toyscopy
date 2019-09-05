#include "gtest/gtest.h"

#include "../core/test.cc"

TEST(example_test, func_sum) { ASSERT_EQ(3, sum(1, 2)); }

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

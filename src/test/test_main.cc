#include "gtest/gtest.h"

#include "../core/test.cc"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

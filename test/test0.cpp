#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"

using namespace spd;

TEST(test0, HelloWorldAssert) {
  Spades spades;
  EXPECT_EQ(spades.getTestNumber(), 1337);
}



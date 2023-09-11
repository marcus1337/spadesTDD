#include <vector>
#include <gtest/gtest.h>
#include "spades.h"

using namespace spds;

TEST(test0, HelloWorldAssert) {
  Spades spades;
  EXPECT_EQ(spades.getTestNumber(), 1337);
}



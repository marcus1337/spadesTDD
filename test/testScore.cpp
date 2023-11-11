#include "test_pch.h"
#include "Spades.h"

using namespace spd;

TEST(Score, storing) {
    Spades spades;
    auto score = spades.getScore();
    EXPECT_EQ(score.first.getPoints(), 0);
    EXPECT_EQ(score.second.getPoints(), 0);
}
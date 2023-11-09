#include "test_pch.h"
#include "Spades.h"

using namespace spd;

TEST(Score, storing) {
    Spades spades;
    auto score = spades.getScore();
    EXPECT_EQ(score.teamScore1.getTotalPoints(), 0);
    EXPECT_EQ(score.teamScore1.getTotalBags(), 0);
}
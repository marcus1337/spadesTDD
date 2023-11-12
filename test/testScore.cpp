#include "test_pch.h"
#include "Spades.h"

using namespace spd;

TEST(Score, storing)
{
    Spades spades;
    spades.reset(0);
    auto score = spades.getScore();
    EXPECT_EQ(score.first.getPoints(), 0);
    EXPECT_EQ(score.second.getPoints(), 0);
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    for (int i = 0; i < 52; i++)
    {
        const auto hand = spades.getHand(spades.getTurnSeat());
        ASSERT_FALSE(hand.empty());
        spades.place(hand.front());
    }
    const auto bags1 = spades.getScore().first.getRoundBags();
    const auto bags2 = spades.getScore().second.getRoundBags();
    
    ASSERT_TRUE(bags1.size() == 1);
    EXPECT_TRUE(bags1.front() + bags2.front() > 0);
}
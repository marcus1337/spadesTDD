#include "test_pch.h"
#include "spades/Spades.h"

using namespace spd;

void finishBids2(Spades &spades, unsigned int bidPerPlayer)
{
    for (int i = 0; i < 4; i++)
    {
        spades.addBid(bidPerPlayer);
    }
}

void showHands(Spades &spades)
{
    for (const auto &seat : SeatUtils::getSeats())
    {
        spades.setBidOption(seat, BidOption::SHOW_HAND);
    }
}

void finishRoundCards(Spades &spades)
{
    for (int i = 0; i < 52; i++)
    {
        spades.place(spades.getPlaceableCards().front());
    }
}

TEST(Score, storing)
{
    Spades spades;
    spades.reset(0);
    auto score = spades.getScore();
    EXPECT_EQ(score.first.getPoints(), 0);
    EXPECT_EQ(score.second.getPoints(), 0);

    finishBids2(spades, 1);
    finishRoundCards(spades);

    const auto bags1 = spades.getScore().first.getRoundBags();
    const auto bags2 = spades.getScore().second.getRoundBags();

    ASSERT_TRUE(bags1.size() == 1);
    EXPECT_TRUE(bags1.front() + bags2.front() > 0);
} 

TEST(Score, points)
{
    Spades spades;
    spades.reset(0);
    showHands(spades);
    //auto score = spades.getScore();
    //EXPECT_EQ(score.first.getPoints(), 0);
    //EXPECT_EQ(score.second.getPoints(), 0);

    for (int i = 0; i < 1; i++)
    {
        finishBids2(spades, 2);
        finishRoundCards(spades);
    }

    auto score = spades.getScore();
    const auto team1Points = score.first.getPoints();
    const auto team2Points = score.second.getPoints();

    //EXPECT_TRUE(team1Points != team2Points);
    //EXPECT_GE(team1Points, 0);
    //EXPECT_GE(team2Points, 0);
}
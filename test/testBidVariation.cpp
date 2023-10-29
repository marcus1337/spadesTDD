#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include "table/Deck.h"
#include <set>
#include <cassert>
#include <algorithm>
#include <ranges>

using namespace spd;

class DoubleBlindNillBidTest : public ::testing::Test
{
protected:
    Spades spades;
    void SetUp() override
    {
        spades.reset(BidVariationType::DOUBLE_BLIND_NILL);
    }
};

TEST_F(DoubleBlindNillBidTest, BlindOption)
{
    const Seat seat = Seat::SOUTH;
    std::vector<BidOption> bidOptions = spades.getBidOptions(seat);
    bool canBlindBid = std::ranges::find(bidOptions, BidOption::SHOW_HAND) != bidOptions.end();
    EXPECT_TRUE(canBlindBid);
    spades.setBidOption(seat, BidOption::SHOW_HAND);
    bidOptions = spades.getBidOptions(seat);
    canBlindBid = std::ranges::find(bidOptions, BidOption::SHOW_HAND) != bidOptions.end();
    EXPECT_FALSE(canBlindBid);
}

TEST_F(DoubleBlindNillBidTest, TeamBidValues)
{
    const Seat seat = Seat::SOUTH;
    const auto possibleBids = spades.getPossibleBids(seat);
    EXPECT_TRUE(possibleBids.size() == 14);
    spades.addBid(13);
    const auto possibleTeamPlayerBids = spades.getPossibleBids(SeatUtils::getTeamSeat(seat));
    EXPECT_TRUE(possibleTeamPlayerBids.size() == 1);
    EXPECT_TRUE(possibleTeamPlayerBids.back() == 0);
}

TEST(MirrorBidTest, TeamBidValues)
{
    Spades spades;
    const unsigned int seed = 0;
    spades.reset(seed);
    spades.reset(BidVariationType::MIRROR);

    int totalSpades = 0;
    for(const auto& seat : SeatUtils::getSeats()){
        const auto possibleBids = spades.getPossibleBids(seat);
        EXPECT_TRUE(possibleBids.size() == 1);
        const auto hand = spades.getHand(seat);
        int numSpades = std::count_if(hand.begin(), hand.end(),
                                        [](const auto &card)
                                        { return card.is(Suit::SPADE); });
        totalSpades += numSpades;
        EXPECT_TRUE(possibleBids.front() == numSpades);
    }
    EXPECT_EQ(totalSpades, 13);    
}

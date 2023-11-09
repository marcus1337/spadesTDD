#include "test_pch.h"
#include "Spades.h"
#include "rules/Deck.h"

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
    for (const auto &seat : SeatUtils::getSeats())
    {
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

class SuicideBidTest : public ::testing::Test
{
protected:
    Spades spades;
    void SetUp() override
    {
        const unsigned int seed = 0;
        spades.reset(seed);
        spades.reset(BidVariationType::SUICIDE);
    }
};

TEST_F(SuicideBidTest, PossibleBids)
{
    const auto seat = spades.getTurnSeat();
    const auto bids = spades.getPossibleBids(seat);
    EXPECT_NO_THROW(bids.at(1));
    EXPECT_EQ(bids.at(1), 4);
    EXPECT_EQ(bids.back(), 13);
    spades.addBid(bids[1]);

    const auto teamBids = spades.getPossibleBids(SeatUtils::getTeamSeat(seat));
    EXPECT_EQ(teamBids.size(), 2);
    EXPECT_EQ(teamBids[0], 0);
    EXPECT_EQ(teamBids[1], bids[1]);

    spades.reset();
    spades.addBid(bids[0]);
    const auto newTeamBids = spades.getPossibleBids(SeatUtils::getTeamSeat(seat));
    EXPECT_EQ(newTeamBids.front(), 4);
}

TEST_F(SuicideBidTest, ConvertingTeamBids)
{
    const auto seat = spades.getTurnSeat();
    EXPECT_FALSE(spades.getBidResult(seat).has_value());
    spades.addBid(4);
    spades.addBid(5);
    spades.addBid(4);
    EXPECT_TRUE(spades.getBidResult(seat).has_value());
    EXPECT_EQ(spades.getBidResult(seat).value(), 0);
}
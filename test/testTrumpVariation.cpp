#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include "table/Deck.h"
#include <set>
#include <cassert>

using namespace spd;

class AceHighTrumpVariation : public ::testing::Test
{
protected:
    State state;
    AceHigh aceHigh;
    void SetUp() override
    {
        state.bids.push_back(1);
        state.bids.push_back(1);
        state.bids.push_back(1);
        state.bids.push_back(1);
    }
};

TEST_F(AceHighTrumpVariation, PlayableCards)
{
}

TEST_F(AceHighTrumpVariation, CardRank)
{
}

TEST_F(AceHighTrumpVariation, LeadCardSuit)
{
}

TEST_F(AceHighTrumpVariation, TrickTakingSeat)
{
    state.playCard(Seat::SOUTH, Card(Rank::KING, Suit::DIAMOND));
    state.playCard(Seat::WEST, Card(Rank::ACE, Suit::CLOVER));
    state.playCard(Seat::NORTH, Card(Rank::KING, Suit::DIAMOND));
    state.playCard(Seat::EAST, Card(Rank::TWO, Suit::SPADE));
    const auto seat = aceHigh.getTrickTaker(state);
    EXPECT_EQ(seat, Seat::EAST);
}

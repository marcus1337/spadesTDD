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
    Spades spades;
    AceHigh aceHigh;
    void SetUp() override
    {
        spades.reset(TrumpVariationType::ACE_HIGH);
        spades.addBid(1);
        spades.addBid(1);
        spades.addBid(1);
        spades.addBid(1);
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
    spades.playCard(Card(Rank::TWO, Suit::DIAMOND));
    spades.playCard(Card(Rank::TEN, Suit::DIAMOND));
    spades.playCard(Card(Rank::KING, Suit::CLOVER));
    spades.playCard(Card(Rank::ACE, Suit::SPADE));
    EXPECT_EQ(spades.getTurnSeat(), Seat::EAST);
}

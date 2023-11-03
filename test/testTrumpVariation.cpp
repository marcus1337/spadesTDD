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
    Spades spades;
    spades.reset(0);
    spades.reset(TrumpVariationType::ACE_HIGH);
    while (spades.isBidPhase())
        spades.addBid(1);
    bool hasNonTrumpCard = false;
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        if (!aceHigh.isTrumpCard(card))
        {
            hasNonTrumpCard = true;
        }
    }
    if (hasNonTrumpCard)
    {
        EXPECT_FALSE(spades.canPlayCard(Card(Rank::ACE, Suit::SPADE)));
    }
}

TEST_F(AceHighTrumpVariation, PlayableCardsLead)
{
    Spades spades;
    spades.reset(0);
    spades.reset(TrumpVariationType::ACE_HIGH);
    while (spades.isBidPhase())
        spades.addBid(1);
    const auto leadCard = spades.getHand(spades.getTurnSeat()).front();
    spades.playCard(leadCard);
    bool hasLeadCardSuit = false;
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        for (const auto &suit : {Suit::SPADE, Suit::DIAMOND, Suit::CLOVER, Suit::HEART})
        {
            if (card.is(suit) && leadCard.is(suit))
                hasLeadCardSuit = true;
        }
    }

    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        for (const auto &suit : {Suit::SPADE, Suit::DIAMOND, Suit::CLOVER, Suit::HEART})
        {
            if (hasLeadCardSuit && card.is(suit) && !leadCard.is(suit))
                EXPECT_FALSE(spades.canPlayCard(card));
            else
                EXPECT_TRUE(spades.canPlayCard(card));
        }
    }
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

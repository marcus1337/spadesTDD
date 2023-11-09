#include "test_pch.h"
#include "Spades.h"
#include "rules/Deck.h"

using namespace spd;

class AceHighTrumpVariation : public ::testing::Test
{
protected:
    State state;
    TrumpVariationController variationController;
    void SetUp() override
    {
        variationController.setTrumpVariationType(TrumpVariationType::ACE_HIGH);
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
        if (!AceHigh().isTrumpCard(card))
        {
            hasNonTrumpCard = true;
        }
    }
    if (hasNonTrumpCard)
    {
        EXPECT_FALSE(spades.canPlace(Card(Rank::ACE, Suit::SPADE)));
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
    spades.place(leadCard);
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
        if (hasLeadCardSuit && !leadCard.is(card.getSuit().value()))
        {
            ASSERT_FALSE(spades.canPlace(card));
        }
        else if (!hasLeadCardSuit)
        {
            ASSERT_TRUE(spades.canPlace(card));
        }
    }
}

TEST_F(AceHighTrumpVariation, TrickTakingSeat)
{
    state.playCard(Seat::SOUTH, Card(Rank::KING, Suit::DIAMOND));
    state.playCard(Seat::WEST, Card(Rank::ACE, Suit::CLOVER));
    state.playCard(Seat::NORTH, Card(Rank::QUEEN, Suit::DIAMOND));
    state.playCard(Seat::EAST, Card(Rank::TWO, Suit::SPADE));
    const auto seat = variationController.getTrickTaker(state);
    EXPECT_EQ(seat, Seat::EAST) << "error, seat was(" << (int)seat << ")";
}

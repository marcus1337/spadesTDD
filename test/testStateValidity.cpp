#include "test_pch.h"
#include "Spades.h"

using namespace spd;

TEST(StateValidity, DissallowCardsBeforeBids)
{
    Spades spades;
    EXPECT_FALSE(spades.isCorrupt());
    const Card card(Rank::ACE, Suit::SPADE);
    spades.place(card);
    EXPECT_TRUE(spades.isCorrupt());
}

TEST(StateValidity, DissallowDuplicateRoundCards)
{
    Spades spades;
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    EXPECT_FALSE(spades.isCorrupt());
    const Card card(Rank::ACE, Suit::SPADE);
    spades.place(card);
    spades.place(card);
    EXPECT_TRUE(spades.isCorrupt());
}

TEST(StateValidity, DissallowHighTeamBids)
{
    Spades spades;
    spades.addBid(14);
    EXPECT_TRUE(spades.isCorrupt());
    spades.reset();
    spades.addBid(7);
    spades.addBid(1);
    spades.addBid(7);
    EXPECT_TRUE(spades.isCorrupt());
}

TEST(StateValidity, UniqueCards)
{
    Spades spades;
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    spades.addBid(1);
    for (int i = 0; i < 52; i++)
    {
        spades.place(spades.getHand(spades.getTurnSeat()).front());
    }
    
    EXPECT_FALSE(spades.isCorrupt());
}
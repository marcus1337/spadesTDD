#include "test_pch.h"
#include "Spades.h"

using namespace spd;

TEST(StateValidity, DissallowCardsBeforeBids)
{
    Spades spades;
    const Card card(Rank::ACE, Suit::SPADE);
    spades.place(card);
    EXPECT_FALSE(spades.isStateValid());
}

TEST(StateValidity, DissallowDuplicateRoundCards)
{
    Spades spades;
    const Card card(Rank::ACE, Suit::SPADE);
    spades.place(card);
    spades.place(card);
    EXPECT_FALSE(spades.isStateValid());
}
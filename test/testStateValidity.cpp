#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>

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
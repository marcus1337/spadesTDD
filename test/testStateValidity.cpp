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

/*TEST(StateValidity, DissallowDuplicateRoundCards)
{
    Spades spades;
    EXPECT_FALSE(spades.isCorrupt());
    const Card card(Rank::ACE, Suit::SPADE);
    spades.place(card);
    spades.place(card);
    EXPECT_TRUE(spades.isCorrupt());
}*/


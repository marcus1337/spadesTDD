#include "test_pch.h"
#include "spades/Spades.h"

using namespace spd;

void finishBids(Spades &spades)
{
    for (int i = 0; i < 4; i++)
    {
        spades.addBid(1);
    }
}

void finishTrick(Spades &spades)
{
    finishBids(spades);
    for (int i = 0; i < 4; i++)
    {
        spades.place(spades.getPlaceableCards().front());
    }
}

bool prevTakerCheck(Spades &spades, const Seat &taker)
{
    return spades.getPrevTrickTaker().has_value() && spades.getPrevTrickTaker().value() == taker;
}

bool nextTakerCheck(Spades &spades, const Seat &taker)
{
    finishTrick(spades);
    return prevTakerCheck(spades, taker);
}

TEST(Score, trick_take)
{
    Spades spades;
    spades.reset(0);
    finishBids(spades);
    spades.place(Card(Rank::TEN, Suit::SPADE));
    spades.place(Card(Rank::SIX, Suit::SPADE));
    spades.place(Card(Rank::KNIGHT, Suit::SPADE));
    spades.place(Card(Rank::THREE, Suit::SPADE));
    EXPECT_TRUE(spades.getPrevTrickTaker().value() == Seat::NORTH);
}

/*EXPECT_TRUE(nextTakerCheck(spades, Seat::NORTH));
EXPECT_TRUE(nextTakerCheck(spades, Seat::WEST));
EXPECT_TRUE(nextTakerCheck(spades, Seat::EAST));
EXPECT_TRUE(nextTakerCheck(spades, Seat::SOUTH));
EXPECT_TRUE(nextTakerCheck(spades, Seat::NORTH));
EXPECT_TRUE(nextTakerCheck(spades, Seat::NORTH));
EXPECT_TRUE(nextTakerCheck(spades, Seat::SOUTH));*/
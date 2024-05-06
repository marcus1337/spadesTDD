#include "test_pch.h"
#include "spades/Spades.h"
using namespace spd;

TEST(UndoRedo, CanUndo)
{
    Spades spades;
    EXPECT_FALSE(spades.canUndo());
    spades.addBid(1);
    ASSERT_TRUE(spades.canUndo()) << "Can't undo";
    spades.undo();
    EXPECT_FALSE(spades.canUndo());
}

TEST(UndoRedo, CanRedo)
{
    Spades spades;
    EXPECT_FALSE(spades.canRedo());
    spades.addBid(1);
    EXPECT_FALSE(spades.canRedo());
    spades.undo();
    EXPECT_TRUE(spades.canRedo());
    spades.addBid(1);
    EXPECT_FALSE(spades.canRedo());
}

TEST(UndoRedo, redo)
{
    Spades spades;
    spades.addBid(1);
    spades.addBid(2);
    spades.undo();
    spades.undo();
    spades.redo();
    EXPECT_EQ(spades.getTurnSeat(), Seat::WEST);
}

TEST(UndoRedo, undoOption)
{
    Spades spades;
    spades.setBidOption(Seat::SOUTH, BidOption::SHOW_HAND);
    spades.setBidOption(Seat::EAST, BidOption::SHOW_HAND);
    spades.setBidOption(Seat::WEST, BidOption::SHOW_HAND);
    spades.setBidOption(Seat::NORTH, BidOption::SHOW_HAND);
    spades.undo();
    spades.undo();
    spades.undo();
    spades.undo();
    EXPECT_EQ(spades.getTurnSeat(), Seat::SOUTH);
}

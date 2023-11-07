#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>

using namespace spd;

TEST(UndoRedo, CanUndo){
    Spades spades;
    EXPECT_FALSE(spades.canUndo());
    spades.addBid(1);
    ASSERT_TRUE(spades.canUndo()) << "Can't undo";
    spades.undo();
    EXPECT_FALSE(spades.canUndo());
}

TEST(UndoRedo, CanRedo){
    Spades spades;
    EXPECT_FALSE(spades.canRedo());
    spades.addBid(1);
    EXPECT_FALSE(spades.canRedo());
    spades.undo();
    EXPECT_TRUE(spades.canRedo());
}
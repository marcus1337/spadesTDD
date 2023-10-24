#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>

using namespace spd;


TEST(Serialization, Seed) {
    Spades spades;
    const auto data = spades.serialize();
    int oldSeed = spades.getSeed();
    spades.setSeed(spades.getSeed()+1);
    spades.deserialize(data);
    EXPECT_EQ(spades.serialize(), data);
    EXPECT_EQ(spades.getSeed(), oldSeed);
    Spades spades2;
    EXPECT_NE(spades2.getSeed(), oldSeed);
}

TEST(API, StartNewGame) {
    Spades spades;
    EXPECT_FALSE(spades.hasStarted());
    spades.startNewGame();
    EXPECT_TRUE(spades.hasStarted());
}

TEST(API, EndGame) {
    Spades spades;
    spades.startNewGame();
    EXPECT_TRUE(spades.hasStarted());
    spades.endGame();
    EXPECT_FALSE(spades.hasStarted());
}

TEST(API, SetBidVariation) {
    Spades spades;
    const auto type = BidVariationType::DOUBLE_NILL;
    spades.setBidVariation(type);
    EXPECT_TRUE(spades.getBidVariationType() == type);
}

TEST(API, SetTrumpVariation) {
    Spades spades;
    const auto type = TrumpVariationType::ACE_HIGH;
    spades.setTrumpVariation(type);
    EXPECT_TRUE(spades.getTrumpVariationType() == type);
}

TEST(API, BidOrder) {
    Spades spades;
    EXPECT_TRUE(spades.getTurnSeat() == Seat::SOUTH);
    EXPECT_FALSE(spades.hasPlayerBid(Seat::SOUTH));
    spades.addBid(1);
    EXPECT_TRUE(spades.getTurnSeat() == Seat::WEST);
    EXPECT_TRUE(spades.hasPlayerBid(Seat::SOUTH));
}

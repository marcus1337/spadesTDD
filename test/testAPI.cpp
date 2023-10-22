#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include "Deck.h"
#include <set>

using namespace spd;


TEST(API, Serialization) {
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
    EXPECT_TRUE(spades.getBidVariation() == type);
}

TEST(API, SetTrumpVariation) {
    Spades spades;
    spades.setTrumpVariation(std::make_unique<AceHigh>());
    EXPECT_TRUE(dynamic_cast<const AceHigh*>(spades.getTrumpVariation()) != nullptr);
}
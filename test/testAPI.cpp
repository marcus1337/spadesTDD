#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include "global/PortableRandom.h"
#include <algorithm>
#include "Deck.h"
#include <set>

using namespace spd;


TEST(API, Serialization) {
    Spades spades;
    const auto data = spades.serialize();
    //TODO: Edit spades state here
    spades.deserialize(data);
    EXPECT_EQ(spades.serialize(), data);
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
    spades.setBidVariation(std::make_unique<DoubleBlindNil>());
    EXPECT_TRUE(dynamic_cast<const DoubleBlindNil*>(spades.getBidVariation()) != nullptr);
}

TEST(API, SetTrumpVariation) {
    Spades spades;
    spades.setTrumpVariation(std::make_unique<AceHigh>());
    EXPECT_TRUE(dynamic_cast<const AceHigh*>(spades.getTrumpVariation()) != nullptr);
}
#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include "global/PortableRandom.h"
#include <algorithm>
#include "Deck.h"
#include <set>

using namespace spd;

TEST(PortableRandom, RandomSame) {
    auto& r = PortableRandom::getInstance();
    std::vector<int> arr1, arr2;
    const int n = 200;
    const int maxValue = 100;
    const int seed = 100;

    r.setSeed(seed);
    for (int i = 0; i < n; i++) {
        arr1.push_back(r.randInt(0, maxValue));
    }
    r.setSeed(seed);
    for (int i = 0; i < n; i++) {
        arr2.push_back(r.randInt(0, maxValue));
    }
    bool areEqual = std::equal(arr1.begin(), arr1.end(), arr2.begin());

    EXPECT_TRUE(areEqual);
}

TEST(PortableRandom, RandomDiff) {
    auto& r = PortableRandom::getInstance();
    std::vector<int> arr1, arr2;
    const int n = 200;
    const int maxValue = 100;
    const int seed1 = 100;
    const int seed2 = seed1+1;

    r.setSeed(seed1);
    for (int i = 0; i < n; i++) {
        arr1.push_back(r.randInt(0, maxValue));
    }
    r.setSeed(seed2);
    for (int i = 0; i < n; i++) {
        arr2.push_back(r.randInt(0, maxValue));
    }
    bool areEqual = std::equal(arr1.begin(), arr1.end(), arr2.begin());

    EXPECT_FALSE(areEqual);
}

TEST(Deck, UniqueCards) {
    Deck deck;
    std::set<Card> cards;
    while (!deck.empty()) {
        const auto card = deck.pop();
        EXPECT_FALSE(cards.contains(card));
        cards.emplace(card);
    }
}

TEST(Deck, Shuffled) {
    Deck deck;
    std::vector<Card> cards;
    while (!deck.empty())
        cards.push_back(deck.pop());
    auto sameSuitInRange = [&cards](int from, int to) {
        const auto firstCard = cards[from];
        for (int i = from; i < to; i++)
            if (!cards[i].sameSuit(firstCard))
                return false;
        return true;
    };
    EXPECT_FALSE(sameSuitInRange(0,12) && sameSuitInRange(13, 25));
}

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
    EXPECT_TRUE(dynamic_cast<DoubleBlindNil*>(spades.getBidVariation()) != nullptr);
}
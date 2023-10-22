#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include "global/PortableRandom.h"
#include <algorithm>
#include "Deck.h"
#include <set>

using namespace spd;

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

#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include "table/Deck.h"
#include <set>

using namespace spd;

TEST(Deck, UniqueCards) {
    Deck deck;
    std::set<Card> cards;
    for(const auto& card : deck.getHand(Seat::SOUTH, 0)){
        EXPECT_FALSE(cards.contains(card));
        cards.emplace(card);
    }
}


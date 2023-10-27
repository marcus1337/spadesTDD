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
    EXPECT_TRUE(deck.getHand(Seat::SOUTH, 0).size() == 13);
    for(const auto& card : deck.getHand(Seat::SOUTH, 0)){
        std::cout << "(" << card.toString() << ") ";
        EXPECT_FALSE(cards.contains(card));
        cards.emplace(card);
    }
}


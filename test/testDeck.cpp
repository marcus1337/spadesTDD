#include "test_pch.h"
#include "Spades.h"
#include "table/Deck.h"

using namespace spd;

TEST(Deck, UniqueCards) {
    Deck deck;
    std::set<Card> cards;
    std::vector<std::vector<Card>> hands;
    for(const auto& seat : {Seat::SOUTH, Seat::EAST, Seat::NORTH, Seat::WEST}){
        const auto hand = deck.getHand(seat, 0);
        EXPECT_TRUE(hand.size() == 13);
        for(const auto& card : hand){
            EXPECT_FALSE(cards.contains(card));
            cards.emplace(card);
        }
    }
}


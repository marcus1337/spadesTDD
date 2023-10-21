#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator> 

namespace spd
{
    enum class Rank {
        ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, KNIGHT, QUEEN, KING, BIG_JOKER, LITTLE_JOKER
    };
    enum class Suit {
        SPADE, HEART, DIAMOND, CLOVER, NONE
    };

    struct Card {
        const Rank rank;
        const Suit suit;
        Card(const Rank& rank, const Suit& suit) : rank(rank), suit(suit) {
            isJoker(rank) ? assert(suit == Suit::NONE) : assert(suit != Suit::NONE);
        }
        static Card BigJoker() {
            return Card(Rank::BIG_JOKER, Suit::NONE);
        }
        static Card LittleJoker() {
            return Card(Rank::LITTLE_JOKER, Suit::NONE);
        }
        static bool isJoker(const Rank& rank) {
            return rank == Rank::BIG_JOKER || rank == Rank::LITTLE_JOKER;
        }
        bool operator==(const Card& other) const {
            return (rank == other.rank) && (suit == other.suit);
        }
    };

}
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

    class Card {
        Rank rank;
        Suit suit;
    public:
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
            return sameRank(other) && sameSuit(other);
        }
        bool operator<(const Card& other) const {
            return sameSuit(other) ? getRank() < other.getRank() : getSuit() < other.getSuit();
        }
        Rank getRank() const {
            return rank;
        }
        Suit getSuit() const {
            return suit;
        }
        bool sameSuit(const Card& other) const {
            return getSuit() == other.getSuit();
        }
        bool sameRank(const Card& other) const {
            return getRank() == other.getRank();
        }
    };

}
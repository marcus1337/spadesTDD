#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <variant>
#include <array>

namespace spd
{
    enum class Rank
    {
        ACE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        KNIGHT,
        QUEEN,
        KING,
        BIG_JOKER,
        LITTLE_JOKER
    };
    enum class Suit
    {
        SPADE,
        HEART,
        DIAMOND,
        CLOVER,
        NONE
    };

    enum class Joker
    {
        BIG,
        LITTLE
    };

    class Card
    {
        Rank rank;
        Suit suit;

    public:
        Card() : rank(Rank::ACE), suit(Suit::SPADE) {}
        Card(const Rank &rank, const Suit &suit) : rank(rank), suit(suit)
        {
            isJoker(rank) ? assert(suit == Suit::NONE) : assert(suit != Suit::NONE);
        }
        static Card BigJoker()
        {
            return Card(Rank::BIG_JOKER, Suit::NONE);
        }
        static Card LittleJoker()
        {
            return Card(Rank::LITTLE_JOKER, Suit::NONE);
        }
        static bool isJoker(const Rank &rank)
        {
            return rank == Rank::BIG_JOKER || rank == Rank::LITTLE_JOKER;
        }
        bool operator==(const Card &other) const
        {
            return sameRank(other) && sameSuit(other);
        }
        bool operator!=(const Card &other) const
        {
            return !(*this == other);
        }
        bool operator<(const Card &other) const
        {
            return sameSuit(other) ? getRank() < other.getRank() : getSuit() < other.getSuit();
        }
        Rank getRank() const
        {
            return rank;
        }
        Suit getSuit() const
        {
            return suit;
        }
        bool sameSuit(const Card &other) const
        {
            return getSuit() == other.getSuit();
        }
        bool sameRank(const Card &other) const
        {
            return getRank() == other.getRank();
        }
        std::string toString() const
        {
            return std::to_string((int)getRank()) + "," + std::to_string((int)getSuit());
        }

        static std::array<Suit, 4> getSuits()
        {
            return {Suit::SPADE, Suit::HEART, Suit::CLOVER, Suit::DIAMOND};
        }
        static std::array<Rank, 13> getRanks()
        {
            std::array<Rank, 13> ranks;
            for (int i = 0; i < ranks.size(); i++)
                ranks[i] = (Rank)i;
            return ranks;
        }

        static std::vector<Card> getDefaultCards()
        {
            std::vector<Card> cards;
            for (const auto &suit : getSuits())
                for (const auto &rank : getRanks())
                    cards.emplace_back(rank, suit);
            return cards;
        }

        static std::vector<Card> getCards()
        {
            std::vector<Card> cards;
            const auto defaultCards = getDefaultCards();
            cards.insert(cards.end(), defaultCards.begin(), defaultCards.end());
            cards.insert(cards.end(), {LittleJoker(), BigJoker()});
            return cards;
        }
    };

}
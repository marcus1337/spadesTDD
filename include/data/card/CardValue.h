#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <variant>
#include <array>
#include "CardValueEnum.h"

namespace spd
{
    class NormalCardValue
    {
        Rank rank;
        Suit suit;

        public:
        NormalCardValue() : rank(Rank::ACE), suit(Suit::SPADE) {}
        NormalCardValue(const Rank &rank, const Suit &suit) : rank(rank), suit(suit)
        {
        }
        Rank getRank() const
        {
            return rank;
        }
        Suit getSuit() const
        {
            return suit;
        }
        bool sameSuit(const NormalCardValue &other) const
        {
            return getSuit() == other.getSuit();
        }
        bool sameRank(const NormalCardValue &other) const
        {
            return getRank() == other.getRank();
        }
        bool operator==(const NormalCardValue &other) const
        {
            return sameRank(other) && sameSuit(other);
        }
        bool operator!=(const NormalCardValue &other) const
        {
            return !(*this == other);
        }
        bool operator<(const NormalCardValue &other) const
        {
            return sameSuit(other) ? getRank() < other.getRank() : getSuit() < other.getSuit();
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

        static std::vector<NormalCardValue> getValues()
        {
            std::vector<NormalCardValue> values;
            for (const auto &suit : getSuits())
                for (const auto &rank : getRanks())
                    values.emplace_back(rank, suit);
            return values;
        }
    };

    class JokerValue
    {
        Joker joker;
        public:
        Joker getJoker() const {
            return joker;
        }
        JokerValue() : joker(Joker::LITTLE) {}
        JokerValue(const Joker &joker) : joker(joker) {}
        bool operator==(const JokerValue &other) const
        {
            return joker == other.getJoker();
        }
        bool operator!=(const JokerValue &other) const
        {
            return !(*this == other);
        }
        bool operator<(const JokerValue &other) const
        {
            return (int)joker < (int)other.getJoker();
        }
    };
    using CardValue = std::variant<NormalCardValue, JokerValue>;
}
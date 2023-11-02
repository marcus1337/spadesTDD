#pragma once

#include <vector>
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
        NormalCardValue();
        NormalCardValue(const Rank &rank, const Suit &suit);
        Rank getRank() const;
        Suit getSuit() const;
        bool sameSuit(const NormalCardValue &other) const;
        bool sameRank(const NormalCardValue &other) const;
        bool operator==(const NormalCardValue &other) const;
        bool operator!=(const NormalCardValue &other) const;
        bool operator<(const NormalCardValue &other) const;
        static std::array<Suit, 4> getSuits();
        static std::array<Rank, 13> getRanks();

        static std::vector<NormalCardValue> getValues();
    };

    class JokerValue
    {
        Joker joker;
        public:
        Joker getJoker() const;
        JokerValue();
        JokerValue(const Joker &joker);
        bool operator==(const JokerValue &other) const;
        bool operator!=(const JokerValue &other) const;
        bool operator<(const JokerValue &other) const;
    };
    using CardValue = std::variant<NormalCardValue, JokerValue>;
}
#pragma once

#include "CardValue.h"
#include <optional>

namespace spd
{

    class Card
    {
        CardValue value;

    public:
        Card();
        explicit Card(int serializedValue);
        Card(const CardValue &value);
        Card(const Rank &rank, const Suit &suit);
        Card(const Joker &joker);

        CardValue getValue() const;

        bool operator==(const Card &other) const;
        bool operator!=(const Card &other) const;
        bool operator<(const Card &other) const;

        int serialize() const;

        static std::vector<Card> getNormalCards();
        static std::vector<Card> getCards();

        std::optional<Suit> getSuit() const;

        bool is(const Rank& rank) const;
        bool is(const Suit &suit) const;
        bool is(const Joker &joker) const;
    };

}
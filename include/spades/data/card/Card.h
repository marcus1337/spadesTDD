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
        explicit Card(unsigned int serializedValue);
        Card(const CardValue &value);
        Card(const Rank &rank, const Suit &suit);
        Card(const Joker &joker);

        CardValue getValue() const;

        bool operator==(const Card &other) const;
        bool operator!=(const Card &other) const;
        bool operator<(const Card &other) const;

        unsigned int serialize() const;

        static std::vector<Card> getNormalCards();
        static std::vector<Card> getCards();

        std::optional<Suit> getSuit() const;
        std::optional<Rank> getRank() const;

        bool sameSuit(const Card& other) const;
        bool is(const Rank& rank) const;
        bool is(const Suit &suit) const;
        bool is(const Joker &joker) const;

        static std::array<Suit, 4> getSuits();
        static std::array<Rank, 13> getRanks();
    };

}
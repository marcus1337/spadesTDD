#pragma once

#include "CardValue.h"
#include "CardValueComparison.h"

namespace spd
{

    class Card
    {
        CardValue value;

    public:
        Card() : value(NormalCardValue()) {}
        Card(const CardValue &value) : value(value) {}
        Card(const Rank &rank, const Suit &suit) : value(NormalCardValue(rank, suit)) {}
        Card(const Joker &joker) : value(JokerValue(joker)) {}

        CardValue getValue() const
        {
            return value;
        }

        bool operator==(const Card &other) const
        {
            return CardValueComparison(getValue()) == other.getValue();
        }
        bool operator!=(const Card &other) const
        {
            return !(*this == other);
        }
        bool operator<(const Card &other) const
        {
            return CardValueComparison(getValue()) < other.getValue();
        }

        static std::vector<Card> getNormalCards()
        {
            std::vector<Card> cards;
            for (const auto &value : NormalCardValue::getValues())
                cards.push_back(Card(value));
            return cards;
        }

        static std::vector<Card> getCards()
        {
            std::vector<Card> cards;
            const auto defaultCards = getNormalCards();
            cards.insert(cards.end(), defaultCards.begin(), defaultCards.end());
            cards.push_back(Card(Joker::LITTLE));
            cards.push_back(Card(Joker::BIG));
            return cards;
        }
    };

}
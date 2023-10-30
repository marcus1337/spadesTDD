#pragma once

#include "CardValue.h"
#include "CardValueComparison.h"
#include "CardValueSerialization.h"

namespace spd
{

    class Card
    {
        CardValue value;

    public:
        Card() : value(NormalCardValue()) {}
        explicit Card(int serializedValue) : value(CardValueSerialization::deserialize(serializedValue)) {}
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

        int serialize() const
        {
            return CardValueSerialization::serialize(getValue());
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

        bool is(const Suit &suit) const
        {
            if (const auto normalValue = std::get_if<NormalCardValue>(&value))
            {
                return normalValue->getSuit() == suit;
            }
            return false;
        }
        bool is(const Joker &joker) const
        {
            if (const auto jokerValue = std::get_if<JokerValue>(&value))
            {
                return jokerValue->getJoker() == joker;
            }
            return false;
        }
    };

}
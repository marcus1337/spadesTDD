#include "spades/data/card/Card.h"
#include "spades/data/card/CardValueComparison.h"
#include "spades/data/card/CardValueSerialization.h"

using namespace spd;

Card::Card() : value(NormalCardValue()) {}
Card::Card(unsigned int serializedValue) : value(CardValueSerialization::deserialize(serializedValue)) {}
Card::Card(const CardValue &value) : value(value) {}
Card::Card(const Rank &rank, const Suit &suit) : value(NormalCardValue(rank, suit)) {}
Card::Card(const Joker &joker) : value(JokerValue(joker)) {}

CardValue Card::getValue() const
{
    return value;
}

bool Card::operator==(const Card &other) const
{
    return CardValueComparison(getValue()) == other.getValue();
}
bool Card::operator!=(const Card &other) const
{
    return !(*this == other);
}
bool Card::operator<(const Card &other) const
{
    return CardValueComparison(getValue()) < other.getValue();
}

unsigned int Card::serialize() const
{
    return CardValueSerialization::serialize(getValue());
}

std::vector<Card> Card::getNormalCards()
{
    std::vector<Card> cards;
    for (const auto &value : NormalCardValue::getValues())
        cards.push_back(Card(value));
    return cards;
}

std::vector<Card> Card::getCards()
{
    std::vector<Card> cards;
    const auto defaultCards = getNormalCards();
    cards.insert(cards.end(), defaultCards.begin(), defaultCards.end());
    cards.push_back(Card(Joker::LITTLE));
    cards.push_back(Card(Joker::BIG));
    return cards;
}

bool Card::is(const Rank &rank) const
{
    if (const auto normalValue = std::get_if<NormalCardValue>(&value))
    {
        return normalValue->getRank() == rank;
    }
    return false;
}

bool Card::is(const Suit &suit) const
{
    if (const auto normalValue = std::get_if<NormalCardValue>(&value))
    {
        return normalValue->getSuit() == suit;
    }
    return false;
}
bool Card::is(const Joker &joker) const
{
    if (const auto jokerValue = std::get_if<JokerValue>(&value))
    {
        return jokerValue->getJoker() == joker;
    }
    return false;
}

bool Card::sameSuit(const Card &other) const
{
    if (getSuit().has_value() && other.getSuit().has_value())
    {
        return getSuit() == other.getSuit();
    }
    return false;
}

std::optional<Suit> Card::getSuit() const
{
    std::optional<Suit> suitOpt;
    if (const auto normalValue = std::get_if<NormalCardValue>(&value))
    {
        return std::make_optional(normalValue->getSuit());
    }
    return suitOpt;
}

std::array<Suit, 4> Card::getSuits()
{
    return {Suit::SPADE, Suit::CLOVER, Suit::DIAMOND, Suit::HEART};
}

std::array<Rank, 13> Card::getRanks()
{
    return {Rank::ACE, Rank::KING, Rank::QUEEN, Rank::KNIGHT, Rank::TEN, Rank::NINE, Rank::EIGHT, Rank::SEVEN, Rank::SIX, Rank::FIVE, Rank::FOUR, Rank::THREE, Rank::TWO};
}

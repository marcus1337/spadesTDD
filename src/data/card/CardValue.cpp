#include "spades/data/card/CardValue.h"
#include <cassert>
#include <algorithm>

using namespace spd;

NormalCardValue::NormalCardValue() : rank(Rank::ACE), suit(Suit::SPADE) {}
NormalCardValue::NormalCardValue(const Rank &rank, const Suit &suit) : rank(rank), suit(suit)
{
}
Rank NormalCardValue::getRank() const
{
    return rank;
}
Suit NormalCardValue::getSuit() const
{
    return suit;
}
bool NormalCardValue::sameSuit(const NormalCardValue &other) const
{
    return getSuit() == other.getSuit();
}
bool NormalCardValue::sameRank(const NormalCardValue &other) const
{
    return getRank() == other.getRank();
}
bool NormalCardValue::operator==(const NormalCardValue &other) const
{
    return sameRank(other) && sameSuit(other);
}
bool NormalCardValue::operator!=(const NormalCardValue &other) const
{
    return !(*this == other);
}
bool NormalCardValue::operator<(const NormalCardValue &other) const
{
    return sameSuit(other) ? getRank() < other.getRank() : getSuit() < other.getSuit();
}
std::array<Suit, 4> NormalCardValue::getSuits()
{
    return {Suit::SPADE, Suit::HEART, Suit::CLOVER, Suit::DIAMOND};
}
std::array<Rank, 13> NormalCardValue::getRanks()
{
    std::array<Rank, 13> ranks;
    for (int i = 0; i < ranks.size(); i++)
        ranks[i] = (Rank)i;
    return ranks;
}

std::vector<NormalCardValue> NormalCardValue::getValues()
{
    std::vector<NormalCardValue> values;
    for (const auto &suit : getSuits())
        for (const auto &rank : getRanks())
            values.emplace_back(rank, suit);
    return values;
}

Joker JokerValue::getJoker() const
{
    return joker;
}
JokerValue::JokerValue() : joker(Joker::LITTLE) {}
JokerValue::JokerValue(const Joker &joker) : joker(joker) {}
bool JokerValue::operator==(const JokerValue &other) const
{
    return joker == other.getJoker();
}
bool JokerValue::operator!=(const JokerValue &other) const
{
    return !(*this == other);
}
bool JokerValue::operator<(const JokerValue &other) const
{
    return (int)joker < (int)other.getJoker();
}
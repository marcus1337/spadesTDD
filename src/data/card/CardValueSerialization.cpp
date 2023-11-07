#include "data/card/CardValueSerialization.h"

using namespace spd;

unsigned int CardValueSerialization::serialize(const CardValue &value)
{
    if (std::holds_alternative<NormalCardValue>(value))
    {
        const auto &normalValue = std::get<NormalCardValue>(value);
        const int suitValue = (int)normalValue.getSuit();
        const int rankValue = (int)normalValue.getRank();
        return (suitValue * maxRankValue) + rankValue;
    }
    if (std::holds_alternative<JokerValue>(value))
    {
        const auto &jokerValue = std::get<JokerValue>(value);
        return maxNormalCardValue + (int)jokerValue.getJoker();
    }
    return 0;
}
CardValue CardValueSerialization::deserialize(unsigned int value)
{
    if (value < maxNormalCardValue)
    {
        const int suitValue = value / maxRankValue;
        const int rankValue = value % maxRankValue;
        Rank rank = (Rank)rankValue;
        Suit suit = (Suit)suitValue;
        return NormalCardValue(rank, suit);
    }
    value -= maxNormalCardValue;
    if (value >= 0 && value <= (int)Joker::BIG)
    {
        return JokerValue((Joker)value);
    }
    return JokerValue();
}
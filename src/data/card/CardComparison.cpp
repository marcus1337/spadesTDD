#include "data/card/CardValueComparison.h"

using namespace spd;

CardValueComparison::CardValueComparison(const CardValue &value) : value(value) {}
bool CardValueComparison::operator==(const CardValue &other) const
{
    if (std::holds_alternative<NormalCardValue>(value) && std::holds_alternative<NormalCardValue>(other))
    {
        const auto &normalValue = std::get<NormalCardValue>(value);
        const auto &otherNormalValue = std::get<NormalCardValue>(other);
        return normalValue == otherNormalValue;
    }
    else if (std::holds_alternative<JokerValue>(value) && std::holds_alternative<JokerValue>(other))
    {
        const auto &jokerValue = std::get<JokerValue>(value);
        const auto &otherJokerValue = std::get<JokerValue>(other);
        return jokerValue == otherJokerValue;
    }
    else
    {
        return false;
    }
}
bool CardValueComparison::operator!=(const CardValue &other) const
{
    return !(*this == other);
}
bool CardValueComparison::operator<(const CardValue &other) const
{
    if (std::holds_alternative<NormalCardValue>(value) && std::holds_alternative<NormalCardValue>(other))
    {
        const auto &normalValue = std::get<NormalCardValue>(value);
        const auto &otherNormalValue = std::get<NormalCardValue>(other);
        return normalValue < otherNormalValue;
    }
    else if (std::holds_alternative<JokerValue>(value) && std::holds_alternative<JokerValue>(other))
    {
        const auto &jokerValue = std::get<JokerValue>(value);
        const auto &otherJokerValue = std::get<JokerValue>(other);
        return jokerValue < otherJokerValue;
    }
    else
    {
        return value != other;
    }
}
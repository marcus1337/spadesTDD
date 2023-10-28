#pragma once
#include "CardValue.h"

namespace spd
{
    struct CardValueComparison
    {
        const CardValue value;
        CardValueComparison(const CardValue &value) : value(value) {}
        bool operator==(const CardValue &other) const
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
        bool operator!=(const CardValue &other) const
        {
            return !(*this == other);
        }
        bool operator<(const CardValue &other) const
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
                return false;
            }
        }
    };
}
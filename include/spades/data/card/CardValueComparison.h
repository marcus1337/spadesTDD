#pragma once
#include "CardValue.h"

namespace spd
{
    struct CardValueComparison
    {
        const CardValue value;
        CardValueComparison(const CardValue &value);
        bool operator==(const CardValue &other) const;
        bool operator!=(const CardValue &other) const;
        bool operator<(const CardValue &other) const;
    };
}
#pragma once
#include "CardValue.h"

namespace spd{

    class CardValueSerialization{
        static constexpr int maxRankValue = 13;
        static constexpr int maxNormalCardValue = 52;
        public:
        static unsigned int serialize(const CardValue& value);
        static CardValue deserialize(unsigned int value);
    };
}
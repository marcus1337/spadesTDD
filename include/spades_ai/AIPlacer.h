#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"

namespace spd
{
    enum class AIStrategy
    {
        ATTACK,
        DEFEND,
        ATTACK_AND_DEFEND,
        RANDOM
    };
    class AIPlacer
    {
    public:
        virtual ~AIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) = 0;
        virtual std::string getName() const = 0;
    };
}

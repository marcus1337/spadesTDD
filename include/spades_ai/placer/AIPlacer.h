#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/logic/Analyze.h"

namespace spd
{
    class AIPlacer
    {
    public:
        virtual ~AIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) = 0;
    };
}

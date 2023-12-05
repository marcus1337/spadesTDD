#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"

namespace spd
{
    class AIAction
    {
        const Spades &spades;
        const Analyze analyze;

    public:
        AIAction(const Spades &spades) : spades(spades), analyze(spades)
        {
        }
    }
}
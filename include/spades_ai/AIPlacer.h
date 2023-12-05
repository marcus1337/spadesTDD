#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"

namespace spd
{
    enum class AIDifficulty
    {
        EASY,
        MEDIUM,
        HARD
    };
    class AIPlacer
    {
    public:
        virtual ~AIPlacer() = default;
        virtual AIDifficulty getDifficulty() const = 0;
        virtual Card getPlacement(const Spades &spades) = 0;
        virtual std::string getName() const = 0;
    };
}

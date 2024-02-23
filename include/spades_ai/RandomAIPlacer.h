#pragma once
#include "spades_ai/AIPlacer.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
    public:
        RandomAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) override;
    };
}
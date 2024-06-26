#pragma once
#include "spades/Spades.h"
#include "spades_ai/placer/RandomAIPlacer.h"
#include "spades_ai/placer/ZeroAIPlacer.h"
#include "spades_ai/placer/TargetAIPlacer.h"
#include "spades_ai/placer/NetAIPlacer.h"

namespace spd
{
    class DefaultPlacer : public AIPlacer
    {
        ZeroAIPlacer zeroPlacer;
        SabotageZeroAIPlacer sabotageZeroPlacer;
        DefendZeroAIPlacer defendZeroPlacer;
        TargetAIPlacer targetAIPlacer;
        const AIPlacer &getPlacer(const Spades &spades) const;

    public:
        DefaultPlacer() = default;
        virtual Card getPlacement(const Spades &spades) const override;
    };
}
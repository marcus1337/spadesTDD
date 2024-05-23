#pragma once
#include "spades_ai/placer/AIPlacer.h"
#include "spades/data/PortableRandom.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
        mutable PortableRandom portableRandom;
        Card getRandomCard(const Spades &spades) const;

    public:
        RandomAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) const override;
    };
}
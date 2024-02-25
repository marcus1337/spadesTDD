#pragma once
#include "spades_ai/logic/AIPlacer.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
        static PortableRandom portableRandom;
        Card getRandomCard(const Spades &spades);

    public:
        RandomAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) override;
    };
}
#pragma once
#include "spades_ai/AIPlacer.h"
#include "spades/data/PortableRandom.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
        PortableRandom portableRandom;
    public:
        RandomAIPlacer() = default;
        virtual AIDifficulty getDifficulty() const override;
        virtual Card getPlacement(const Spades &spades) override;
        virtual std::string getName() const override;
    };
}
#pragma once
#include "spades_ai/AIPlacer.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
    public:
        RandomAIPlacer() = default;
        virtual AIDifficulty getDifficulty() const override;
        virtual Card getPlacement(const Spades &spades) override;
        virtual std::string getName() const override;
    };
}
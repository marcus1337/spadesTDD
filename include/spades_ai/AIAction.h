#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"
#include "spades/data/PortableRandom.h"

namespace spd
{
    class AIAction
    {
        static PortableRandom portableRandom;
        const Spades &spades;

        Card getCard(const std::vector<float>& netOutput, const std::array<std::pair<int, float>, 4>& suitIndices) const;
        std::array<std::pair<int, float>, 4> getSuitIndices(const std::vector<float>& netOutput) const;
    public:
        AIAction(const Spades &spades);
        Card getRandomCard();
        Card getCard(const std::vector<float>& netOutput) const;
    };
}
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
        const Analyze analyze;

    public:
        AIAction(const Spades &spades) : spades(spades), analyze(spades)
        {
        }

        Card getRandomCard()
        {
            const auto placeableCards = spades.getPlaceableCards();
            assert(!placeableCards.empty());
            const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
            return placeableCards[index];
        }
        
    };
}
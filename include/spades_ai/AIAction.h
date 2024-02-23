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
        std::vector<Card> getPlaceableCardsOrderedByStrengthDescending(const Suit& suit) const;

    public:
        AIAction(const Spades &spades);
        Card getRandomCard();
        Card getCard(std::vector<float> netOutput) const;
    };
}
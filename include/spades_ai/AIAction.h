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

    public:
        AIAction(const Spades &spades);
        Card getRandomCard();

        std::optional<Card> getHighestWinCard() const;  //TODO: impl
        std::optional<Card> getLowestWinCard() const;   //TODO: impl
        std::optional<Card> getHighestLoseCard() const; //TODO: impl
        std::optional<Card> getLowestLoseCard() const;  //TODO: impl
    };
}
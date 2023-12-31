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

        std::vector<Card> getPlaceableWinCards() const;
        std::vector<Card> getPlaceableLoseCards() const;
        std::vector<Card> copyCardsOrderedByStrengthDescending(const std::vector<Card>& cards) const;

    public:
        AIAction(const Spades &spades);
        Card getRandomCard();

        std::optional<Card> getHighestWinCard() const;
        std::optional<Card> getLowestWinCard() const;
        std::optional<Card> getHighestLoseCard() const;
        std::optional<Card> getLowestLoseCard() const;
    };
}
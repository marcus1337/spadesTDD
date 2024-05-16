#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class Observation
    {
        const Spades &spades;

        bool hasSkippedLeadSuit(const Suit& leadSuit, const Seat& seat) const;
        std::vector<Card> filterCards(const Suit& suit, const std::vector<Card>& cards) const;

    public:
        Observation(const Spades &spades);

        std::vector<float> getNetInput() const;
    };
}
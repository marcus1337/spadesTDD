#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class AIObservation
    {
        const Spades &spades;

        bool isLastTrickCard() const;
        bool canPlaceTopCard(const Seat& seat) const;
        bool canPlaceNonTopCard(const Seat &seat) const;
        bool isNilBidAtRisk(const Seat& seat) const;

    public:
        AIObservation(const Spades &spades);
        bool isDefendingNil(const Seat &seat) const;
        bool canSabotageOpponentNil() const;
        bool canDefendTeamNil() const;
        bool teamNeedTricks() const;
        bool opponentTeamNeedTricks() const;
    };
}
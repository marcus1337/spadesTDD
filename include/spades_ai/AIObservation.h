#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"

namespace spd
{
    class AIObservation
    {
        const Spades &spades;
        const Analyze analyze;

        bool isLastTrickCard() const;
        bool canPlaceNonTopCard(const Seat &seat) const;
        bool hasPlacedCard(const Seat &seat) const;
        bool isDefendingNil(const Seat &seat) const;
        int getCountedRoundTricks(const Seat &seat) const;
        int getCountedRoundTeamTricks(const Seat &seat) const;
        int getTeamBid(const Seat &seat) const;

    public:
        AIObservation(const Spades &spades);
        bool canSabotageOpponentNil() const;
        bool isDefendingNil() const;
        bool isDefendingTeamNil() const;
        bool isOpponentDefendingNil() const;
        bool needMoreTricks() const;
        bool teamNeedMoreTricks() const;
        bool opponentNeedMoreTricks() const;
    };
}
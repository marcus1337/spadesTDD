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

    public:
        AIObservation(const Spades &spades) : spades(spades), analyze(spades)
        {
        }

        bool canSabotageOpponentNil() const
        {
            const auto seat = spades.getTurnSeat();
            const auto rightOpponent = SeatUtils::getRightOpponentSeat(seat);
            const auto leftOpponent = SeatUtils::getLeftOpponentSeat(seat);
            const auto trickSeatCards = spades.getCurrentRoundCardSeatPairs();
            
            if (trickSeatCards.size() == NUM_SEATS - 1)
            {

            }
            return false;
        }
        bool isDefendingNil() const
        {
            return false;
        }
        bool isDefendingTeamNil() const
        {
            return false;
        }
        bool isOpponentDefendingNil() const
        {
            return false;
        }
        bool needMoreTricks() const
        {
            return false;
        }
        bool teamNeedMoreTricks() const
        {
            return false;
        }
        bool opponentNeedMoreTricks() const
        {
            return false;
        }
    };
}
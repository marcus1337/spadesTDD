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

        bool canLoseTrick() const
        {
            return false;
        }

        bool canWinTrick() const
        {
            return false;
        }

        bool isDefendingNil(const Seat &seat) const
        {
            const int numTakenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
            const bool nilBid = spades.getBidResult(seat).value();
            return nilBid && numTakenTricks == 0;
        }

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
                for (const auto &opponent : {rightOpponent, leftOpponent})
                {
                    if (spades.getBidResult(opponent).value() == 0 && spades.getCurrentTrickTopSeat().value() == opponent)
                    {
                        return canLoseTrick();
                    }
                }
            }
            return false;
        }

        bool isDefendingNil() const
        {
            return isDefendingNil(spades.getTurnSeat());
        }
        bool isDefendingTeamNil() const
        {
            return isDefendingNil(SeatUtils::getTeamSeat(spades.getTurnSeat()));
        }
        bool isOpponentDefendingNil() const
        {
            const auto seat = spades.getTurnSeat();
            return isDefendingNil(SeatUtils::getLeftOpponentSeat(seat)) || isDefendingNil(SeatUtils::getRightOpponentSeat(seat));
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
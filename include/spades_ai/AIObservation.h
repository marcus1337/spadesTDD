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

        bool canPlaceNonTopCard(const Seat &seat) const
        {
            for (const auto &card : spades.getHand(seat))
            {
                if (!spades.isTopCardIfPlaced(card))
                {
                    return true;
                }
            }
            return false;
        }

        bool hasPlacedCard(const Seat &seat) const
        {
            for (const auto &[trickSeat, card] : spades.getCurrentTrickCardSeatPairs())
            {
                if (trickSeat == seat)
                {
                    return true;
                }
            }
            return false;
        }

        bool isDefendingNil(const Seat &seat) const
        {
            const int numTakenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
            const bool nilBid = spades.getBidResult(seat).value();
            return nilBid && numTakenTricks == 0;
        }

        int getCountedRoundTricks(const Seat &seat) const
        {
            int bid = spades.getBidResult(seat).value_or(0);
            if (bid == 0)
            {
                return 0;
            }
            else
            {
                return spades.getNumberOfTakenTricksCurrentRound(seat);
            }
        }

        int getCountedRoundTeamTricks(const Seat &seat) const
        {
            const Seat teamSeat = SeatUtils::getTeamSeat(seat);
            return getCountedRoundTricks(seat) + getCountedRoundTricks(teamSeat);
        }

        int getTeamBid(const Seat &seat) const
        {
            const Seat teamSeat = SeatUtils::getTeamSeat(seat);
            return spades.getBidResult(seat).value_or(0) + spades.getBidResult(teamSeat).value_or(0);
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
                        return canPlaceNonTopCard(seat);
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
            const auto seat = spades.getTurnSeat();
            const auto bid = spades.getBidResult(seat);
            if (bid == 0)
            {
                return false;
            }
            else
            {
                return bid > getCountedRoundTeamTricks(seat);
            }
        }

        bool teamNeedMoreTricks() const
        {
            const auto seat = spades.getTurnSeat();
            return getTeamBid(seat) > getCountedRoundTeamTricks(seat);
        }
        bool opponentNeedMoreTricks() const
        {
            const auto seat = SeatUtils::getLeftOpponentSeat(spades.getTurnSeat());
            return getTeamBid(seat) > getCountedRoundTeamTricks(seat);
        }
    };
}
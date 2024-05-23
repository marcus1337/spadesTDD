#pragma once
#include "spades_ai/placer/AIPlacer.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/AIAction.h"

// Common sense decisions when trying to take enough tricks to cover team bid but not more
// also assume there are no 0-bids

namespace spd
{

    class TargetAIPlacer : public AIPlacer
    {
        int getTargetBid(const Spades &spades) const
        {
            const auto seat = spades.getTurnSeat();
            return spades.getBidResult(seat).value() + spades.getBidResult(SeatUtils::getTeamSeat(seat)).value();
        }

        int getTeamTakenTricks(const Spades &spades) const
        {
            const auto seat = spades.getTurnSeat();
            const auto team = SeatUtils::getTeamSeat(seat);
            int counter = 0;
            return spades.getCountedRoundTricks(seat) + spades.getCountedRoundTricks(team);
        }

        bool needTricks(const Spades &spades) const
        {
            return getTargetBid(spades) < getTeamTakenTricks(spades);
        }

    public:
        virtual Card getPlacement(const Spades &spades) const override
        {
            if (needTricks(spades))
            {
                return AIAction(spades).getTakeTricksCard();
            }
            else
            {
                return AIAction(spades).getAvoidTricksCard();
            }
        }
    };
}
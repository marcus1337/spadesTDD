#pragma once
#include "spades_ai/placer/AIPlacer.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/AIAction.h"

// Common sense decisions when having made the zero-bid.

namespace spd
{
    class ZeroAIPlacer : public AIPlacer
    {

    public:
        ZeroAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) const override
        {
            AIAction action(spades);
            if (spades.getCurrentRoundCardSeatPairs().empty())
            {
                return action.getLowestCardFromLeastOwnedSuit();
            }
            else
            {
                const auto &highestLose = action.getHighesLoseCard();
                if (highestLose.has_value())
                {
                    return highestLose.value();
                }
                else
                {
                    return action.getLowestWinCard().value();
                }
            }
        }
    };

    class SabotageZeroAIPlacer : public AIPlacer // Assume 1 or more opponents has an active 0-bids
    {
        virtual Card getPlacement(const Spades &spades) const override
        {
            return AIAction(spades).getAvoidTricksCard();
        }
    };
    class DefendZeroAIPlacer : public AIPlacer // Assume team member has active 0-bid but not self
    {
        virtual Card getPlacement(const Spades &spades) const override
        {
            return AIAction(spades).getTakeTricksCard();
        }
    };
}
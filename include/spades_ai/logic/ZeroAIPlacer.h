#pragma once
#include "spades_ai/logic/AIPlacer.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/AIAction.h"

// Common sense decisions when having made the zero-bid.

namespace spd
{
    class ZeroAIPlacer : public AIPlacer
    {

    public:
        ZeroAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) override
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
        // place highest cards if must win, place highest lose card if can lose, place low card if may lose
    };
    class DefendZeroAIPlacer : public AIPlacer // Assume team member has active 0-bid but not self
    {
        // place lowest cards if must lose, place low win card if may win, place high card if may win
    };

}
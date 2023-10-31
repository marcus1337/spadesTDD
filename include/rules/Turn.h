#pragma once
#include "rules/BidVariation.h"
#include "rules/TrumpVariation.h"
#include "data/State.h"

namespace spd
{
    class Turn
    {

    public:
        Seat getTurnSeat(const State &state) const
        {
            const int round = state.getRound();
            const auto playedRoundSeatCardPairs = state.getPlayedCards(round);
            const int startBidIndex = round % SeatUtils::numSeats;

            if (state.isBidPhase())
            {
                int playerIndex = (state.bids.size() + startBidIndex) % SeatUtils::numSeats;
                return (Seat)playerIndex;
            }
            else if (state.trickTakers.empty())
            {
                return (Seat)playedRoundSeatCardPairs.size();
            }
            else
            {
                const auto startSeat = state.trickTakers.back();
                const int numTrickSteps = state.getPlayedTrickCardSeatPairs().size();
                return SeatUtils::getNextSeats(startSeat, numTrickSteps).back();
            }
        }
    };

}
#include "rules/Turn.h"

using namespace spd;

Seat Turn::getTurnSeat(const State &state) const
{
    const int round = state.getRound();
    const auto playedRoundSeatCardPairs = state.getPlayedCardSeatPairs(round);
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

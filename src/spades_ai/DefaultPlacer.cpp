#include "spades_ai/placer/DefaultPlacer.h"

using namespace spd;
const AIPlacer &DefaultPlacer::getPlacer(const Spades &spades) const
{
    const auto seat = spades.getTurnSeat();
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    const auto opp1 = SeatUtils::getLeftOpponentSeat(seat);
    const auto opp2 = SeatUtils::getRightOpponentSeat(seat);
    const auto numTakenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    const auto numTakenTricksTeamPlayer = spades.getNumberOfTakenTricksCurrentRound(teamSeat);
    const auto bid = spades.getBidResult(seat).value();
    const auto teamBid = spades.getBidResult(teamSeat).value();
    const auto opp1Bid = spades.getBidResult(opp1).value();
    const auto opp2Bid = spades.getBidResult(opp2).value();
    const auto numTricksOpp1 = spades.getNumberOfTakenTricksCurrentRound(opp1);
    const auto numTricksOpp2 = spades.getNumberOfTakenTricksCurrentRound(opp2);
    const bool enemyIsDefendingNil = (opp1Bid == 0 && numTricksOpp1 == 0) || (opp2Bid == 0 && numTricksOpp2 == 0);

    if (bid == 0 && numTakenTricks == 0)
    {
        return zeroPlacer;
    }
    else if (teamBid == 0 && numTakenTricksTeamPlayer == 0)
    {
        return defendZeroPlacer;
    }
    else if (enemyIsDefendingNil)
    {
        return sabotageZeroPlacer;
    }
    else
    {
        return targetAIPlacer;
    }
}
Card DefaultPlacer::getPlacement(const Spades &spades) const
{
    return getPlacer(spades).getPlacement(spades);
}
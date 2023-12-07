#include "spades_ai/AIObservation.h"

using namespace spd;

bool AIObservation::isLastTrickCard() const
{
    return spades.getCurrentTrickCardSeatPairs().size() == NUM_SEATS - 1;
}

bool AIObservation::canPlaceTopCard(const Seat &seat) const
{
    for (const auto &card : spades.getHand(seat))
    {
        if (spades.isTopCardIfPlaced(card))
        {
            return true;
        }
    }
    return false;
}

bool AIObservation::canPlaceNonTopCard(const Seat &seat) const
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

bool AIObservation::isDefendingNil(const Seat &seat) const
{
    const int numTakenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    const bool nilBid = spades.getBidResult(seat).value();
    return nilBid && numTakenTricks == 0;
}

bool AIObservation::isNilBidAtRisk(const Seat &seat) const
{
    const auto topSeat = spades.getCurrentTrickTopSeat();
    return topSeat.has_value() && topSeat.value() == seat && isDefendingNil(seat);
}

AIObservation::AIObservation(const Spades &spades) : spades(spades)
{
}

bool AIObservation::canSabotageOpponentNil() const
{
    const auto seat = spades.getTurnSeat();
    if (isLastTrickCard())
    {
        for (const auto &opponent : {SeatUtils::getRightOpponentSeat(seat), SeatUtils::getLeftOpponentSeat(seat)})
        {
            if (isNilBidAtRisk(opponent))
            {
                return canPlaceNonTopCard(seat);
            }
        }
    }
    return false;
}

bool AIObservation::canDefendTeamNil() const
{
    const auto seat = spades.getTurnSeat();
    if (isNilBidAtRisk(SeatUtils::getTeamSeat(seat)))
    {
        return canPlaceTopCard(seat);
    }
    else
    {
        return false;
    }
}

bool AIObservation::teamNeedTricks() const
{
    const auto seat = spades.getTurnSeat();
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    const int countedTricks = spades.getCountedRoundTricks(seat) + spades.getCountedRoundTricks(teamSeat);
    const int teamBid = spades.getBidResult(seat).value() + spades.getBidResult(teamSeat).value();
    return teamBid > countedTricks;
}

bool AIObservation::opponentTeamNeedTricks() const
{
    const auto leftOpponent = SeatUtils::getLeftOpponentSeat(spades.getTurnSeat());
    const auto rightOpponent = SeatUtils::getRightOpponentSeat(spades.getTurnSeat());
    const int countedTricks = spades.getCountedRoundTricks(leftOpponent) + spades.getCountedRoundTricks(rightOpponent);
    const int opponentTeamBid = spades.getBidResult(leftOpponent).value() + spades.getBidResult(rightOpponent).value();
    return opponentTeamBid > countedTricks;
}

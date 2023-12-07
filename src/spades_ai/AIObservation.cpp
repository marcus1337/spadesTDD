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

int AIObservation::getCountedRoundTricks(const Seat &seat) const
{
    if (spades.getBidResult(seat).value_or(0) == 0)
    {
        return 0;
    }
    else
    {
        return spades.getNumberOfTakenTricksCurrentRound(seat);
    }
}

int AIObservation::getCountedRoundTeamTricks(const Seat &seat) const
{
    const Seat teamSeat = SeatUtils::getTeamSeat(seat);
    return getCountedRoundTricks(seat) + getCountedRoundTricks(teamSeat);
}

int AIObservation::getTeamBid(const Seat &seat) const
{
    const Seat teamSeat = SeatUtils::getTeamSeat(seat);
    return spades.getBidResult(seat).value_or(0) + spades.getBidResult(teamSeat).value_or(0);
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

bool AIObservation::needMoreTricks() const
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

bool AIObservation::teamNeedMoreTricks() const
{
    const auto seat = spades.getTurnSeat();
    return getTeamBid(seat) > getCountedRoundTeamTricks(seat);
}
bool AIObservation::opponentNeedMoreTricks() const
{
    const auto seat = SeatUtils::getLeftOpponentSeat(spades.getTurnSeat());
    return getTeamBid(seat) > getCountedRoundTeamTricks(seat);
}

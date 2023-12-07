#include "spades_ai/AIObservation.h"

using namespace spd;

bool AIObservation::isLastTrickCard() const
{
    return spades.getCurrentTrickCardSeatPairs().size() == NUM_SEATS - 1;
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

bool AIObservation::hasPlacedCard(const Seat &seat) const
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

AIObservation::AIObservation(const Spades &spades) : spades(spades), analyze(spades)
{
}

bool AIObservation::canSabotageOpponentNil() const
{
    const auto seat = spades.getTurnSeat();
    if (isLastTrickCard())
    {
        for (const auto &opponent : {SeatUtils::getRightOpponentSeat(seat), SeatUtils::getLeftOpponentSeat(seat)})
        {
            if (isDefendingNil(opponent) && spades.getCurrentTrickTopSeat().value() == opponent)
            {
                return canPlaceNonTopCard(seat);
            }
        }
    }
    return false;
}

bool AIObservation::isDefendingNil() const
{
    return isDefendingNil(spades.getTurnSeat());
}
bool AIObservation::isDefendingTeamNil() const
{
    return isDefendingNil(SeatUtils::getTeamSeat(spades.getTurnSeat()));
}
bool AIObservation::isOpponentDefendingNil() const
{
    const auto seat = spades.getTurnSeat();
    return isDefendingNil(SeatUtils::getLeftOpponentSeat(seat)) || isDefendingNil(SeatUtils::getRightOpponentSeat(seat));
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
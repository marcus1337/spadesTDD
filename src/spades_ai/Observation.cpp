#include "spades_ai/logic/Observation.h"

using namespace spd;

Observation::Observation(const Spades &spades) : observation{}
{
    seat = spades.getTurnSeat();
    oppSeat1 = SeatUtils::getLeftOpponentSeat(seat);
    oppSeat2 = SeatUtils::getRightOpponentSeat(seat);
    teamSeat = SeatUtils::getTeamSeat(seat);
}

const std::array<float, Observation::OBSERVATION_SIZE> &Observation::getValues() const
{
    return observation;
}

bool Observation::hasStartedTrickBreakingSpades(const Spades &spades, const Seat &seat) const
{
    if (!spades.isSpadesBroken())
    {
        return false;
    }
    uint64_t counter = 0;
    for (const auto &pair : spades.getCurrentRoundCardSeatPairs())
    {
        counter++;
        if (spades.getEffectiveSuit(pair.second) == Suit::SPADE)
        {
            return counter % 4 == 0 && pair.first == seat;
        }
    }
    return false;
}

bool Observation::wasFirstSpadePlacedStartOfTrick(const Spades &spades) const
{
    uint64_t counter = 0;
    for (const auto &pair : spades.getCurrentRoundCardSeatPairs())
    {
        counter++;
        if (spades.getEffectiveSuit(pair.second) == Suit::SPADE)
        {
            return counter % 4 == 0;
        }
    }
    return false;
}

bool Observation::areAllSuitCardsPlaced(const Spades &spades, const Suit &suit) const
{
    for (const auto &card : spades.getUnplacedRoundCards())
    {
        const auto &unplacedSuit = spades.getEffectiveSuit(card);
        if (unplacedSuit == suit)
        {
            return false;
        }
    }
    return true;
}

bool Observation::hasSkippedLeadSuit(const Spades &spades, const std::vector<std::pair<Seat, Card>> &csPairs, int from, const Suit &suit, const Seat &seat) const
{
    const Suit leadSuit = spades.getEffectiveSuit(csPairs[from].second);
    for (int i = 1; i <= 3 && i + from < csPairs.size(); i++)
    {
        const auto &otherSeat = csPairs[from + i].first;
        const auto &otherSuit = spades.getEffectiveSuit(csPairs[from + i].second);
        if (otherSeat == seat && otherSuit != leadSuit)
        {
            return true;
        }
    }
    return false;
}

bool Observation::hasSkippedLeadSuit(const Spades &spades, const Suit &suit, const Seat &seat) const
{
    const auto &csPairs = spades.getCurrentRoundCardSeatPairs();
    const auto cardsPerTrick = 4;
    for (int i = 0; i < spades.getRound() + 1; i++)
    {
        const auto from = i * cardsPerTrick;
        if (from < csPairs.size() && hasSkippedLeadSuit(spades, csPairs, from, suit, seat))
        {
            return true;
        }
    }
    return false;
}

bool Observation::hasActiveNilBid(const Spades &spades, const Seat &seat) const
{
    const auto &bid = spades.getBidResult(seat).value();
    const auto &takenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    return bid == 0 && takenTricks == 0;
}

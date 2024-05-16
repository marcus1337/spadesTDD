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
    return false; // TODO
}
bool Observation::areAllSuitCardsPlaced(const Spades &spades, const Suit &suit) const
{
    return false; // TODO
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
    return false; // TODO
}

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

bool Observation::hasSkippedLeadSuit(const Spades &spades, const Suit &leadSuit, const Seat &seat) const
{ // TODO: Make it work for half-finished tricks
    const auto &csPairs = spades.getCurrentRoundCardSeatPairs();
    for (int trickIndex = 0; trickIndex < csPairs.size() / 4; trickIndex++)
    {
        int from = trickIndex * 4;
        const Card &leadCard = csPairs[from].second;
        if (leadCard.is(leadSuit))
        {
            for (int i = 1; i <= 3; i++)
            {
                const auto &otherSeat = csPairs[from + i].first;
                const auto &otherCard = csPairs[from + i].second;
                if (otherSeat == seat && !leadCard.is(leadSuit))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

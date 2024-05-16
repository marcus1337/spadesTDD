#include "spades_ai/logic/Observation.h"

using namespace spd;

Observation::Observation(const Spades &spades) : spades(spades)
{
}


std::vector<float> Observation::getNetInput() const
{
    std::vector<float> input;
    input.push_back(spades.isSpadesBroken() ? 1.f : 0);

    const auto seat = spades.getTurnSeat();
    const auto opp1 = SeatUtils::getLeftOpponentSeat(seat);
    const auto opp2 = SeatUtils::getRightOpponentSeat(seat);
    const auto teamSeat = SeatUtils::getTeamSeat(seat);


    return input;
}


bool Observation::hasSkippedLeadSuit(const Suit &leadSuit, const Seat &seat) const
{ // TODO: Make it work for half-finished tricks
    const auto csPairs = spades.getCurrentRoundCardSeatPairs();
    for (int trickIndex = 0; trickIndex < csPairs.size() / 4; trickIndex++)
    {
        int from = trickIndex * 4;
        const Card& leadCard = csPairs[from].second;
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


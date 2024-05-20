#include "spades_ai/logic/SuitSupplyObservation.h"
#include "spades_ai/logic/NetUtils.h"

using namespace spd;

SuitSupplyObservation::SuitSupplyObservation(const Spades &spades) : values{}
{
    setKnownAbsentSuits(spades);
}

const std::array<float, 4 * (NUM_SEATS - 1)> &SuitSupplyObservation::getValues() const
{
    return values;
}

std::size_t SuitSupplyObservation::getValueIndex(unsigned int seatIndex, const Suit &suit) const
{
    return seatIndex * 4 + (unsigned int)suit;
}

void SuitSupplyObservation::setKnownAbsentSuits(const Spades &spades)
{
    for (const auto &otherSeat : SeatUtils::getOtherSeats(spades.getTurnSeat()))
    {
        const auto &seatIndex = net::getRelativeSeatIndex(spades, otherSeat);

        for (const auto &suit : Card::getSuits())
        {
            values[getValueIndex(seatIndex, suit)] = net::areAllSuitCardsPlaced(spades, suit) || net::hasSkippedLeadSuit(spades, suit, otherSeat) ? 1.f : 0.f;
        }
        if (net::hasStartedTrickBreakingSpades(spades, otherSeat))
        {
            for (const auto &suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART})
            {
                values[getValueIndex(seatIndex, suit)] = 1.f;
            }
        }
    }
}
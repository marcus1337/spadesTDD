#include "spades_ai/logic/SuitSupplyObservation.h"
#include "spades_ai/logic/NetUtils.h"

using namespace spd;

SuitSupplyObservation::SuitSupplyObservation(const Spades &spades) : values{}, suitTally(spades), spadeBreaker(spades.getSpadeBreaker(spades.getRound()))
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
        setKnownAbsentSuits(spades, otherSeat);
    }
}

void SuitSupplyObservation::setKnownAbsentSuits(const Spades &spades, const Seat &otherSeat)
{
    const auto &seatIndex = net::getRelativeSeatIndex(spades, otherSeat);

    for (const auto &suit : Card::getSuits())
    {
        if (!suitTally.otherSeatHasSuit(suit) || net::hasSkippedLeadSuit(spades, suit, otherSeat))
        {
            values[getValueIndex(seatIndex, suit)] = 1.f;
        }
    }

    if (spadeBreaker.has_value() && spadeBreaker.value() == otherSeat && net::hasStartedTrickBreakingSpades(spades, otherSeat))
    {
        for (const auto &suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART})
        {
            values[getValueIndex(seatIndex, suit)] = 1.f;
        }
    }
}
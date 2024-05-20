#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class SuitSupplyObservation
    {
    public:
        SuitSupplyObservation(const Spades &spades);
        const std::array<float, 4 * (NUM_SEATS - 1)> &getValues() const;

    private:
        std::array<float, 4 * (NUM_SEATS - 1)> values;
        void setKnownAbsentSuits(const Spades &spades);
        std::size_t getValueIndex(unsigned int seatIndex, const Suit &suit) const;
    };
}
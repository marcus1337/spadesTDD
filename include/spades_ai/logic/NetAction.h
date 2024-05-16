#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class NetAction
    {
        static constexpr auto OUTPUT_SIZE = 52; // possible cards
        Card card = Card(0);

        std::vector<unsigned int> getIndicesSortedByMax(const std::array<float, OUTPUT_SIZE> &output) const;
        Card selectCard(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output) const;

    public:
        NetAction(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output);
        Card getPlacement() const;
    };
}
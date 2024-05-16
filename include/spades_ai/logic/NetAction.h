#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class NetAction
    {
        static constexpr auto OUTPUT_SIZE = 52; // possible cards
        Card card = Card(0);

    public:
        NetAction(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output)
        {
        }
        Card getPlacement() const
        {
            return card;
        }
    };
}
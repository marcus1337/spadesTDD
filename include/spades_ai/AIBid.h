#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class AIBid
    {
        static int getClosestNonZeroBid(const std::vector<int> possibleBids, int target);

    public:

        static int getBid(const Spades& spades);
    };

}
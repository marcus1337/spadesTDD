#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"

namespace spd
{
    class AIBid
    {
        const Spades& spades;
        Analyze analyze;
        int getClosestNonZeroBid(int targetBid) const;
        bool hasWeakHand() const;

    public:
        AIBid(const Spades& spades);
        int getBid() const;
    };

}
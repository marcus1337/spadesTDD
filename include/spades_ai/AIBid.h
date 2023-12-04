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
        int getNumStrongCards() const;
        int getNumStrongNonTrumpCards() const;
        bool isStrongStartCard(const Card& card) const;
        bool cardIsOfRank(const Card& card, const std::set<Rank>& possibleRanks) const;
        std::set<Rank> getStrongRanks() const;
        bool cardHasStrongRank(const Card& card) const;
    public:
        AIBid(const Spades& spades);
        int getBid() const;
    };

}
#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class Analyze
    {
        const Spades &spades;
        std::set<Suit> getUnfollowedEffectiveLeadSuits(const Seat &targetSeat) const;
        int getTrumpIndexDescending(const Card& card) const;

    public:
        Analyze(const Spades &spades);

        std::vector<Card> getPlayedRoundCards(const Seat &targetSeat) const;
        std::vector<Card> getRemainingNonSeatRoundCards(const Seat &seat) const;
        bool isEffectiveSuitInOtherHand(const Seat &perspectiveSeat, const Suit &suit) const;
        bool isEffectiveSuitInAnyHand(const Suit &suit) const;

        std::set<Suit> getSelfEffectiveSuits(const Seat &self) const;
        std::set<Suit> getSelfEffectiveVoidSuits(const Seat &self) const;
        std::set<Suit> getVoidEffectiveSuits(const Seat &perspectiveSeat, const Seat &targetSeat) const;
        std::set<Suit> getEffectiveSuitsFromElimination(const Seat &perspectiveSeat, const Seat &targetSeat) const;
    
        int getGuaranteedTrickTakes(const Seat& seat) const;
    };
}
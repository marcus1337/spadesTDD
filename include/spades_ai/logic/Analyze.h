#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class Analyze
    {
        const Spades &spades;
        int getTrumpIndexDescending(const Card &card) const;

    public:
        Analyze(const Spades &spades);

        std::vector<Card> getPlayedRoundCards(const Seat &targetSeat) const;
        std::vector<Card> getRemainingNonSeatRoundCards(const Seat &seat) const;
        int getGuaranteedTrickTakes(const Seat &seat) const;

        int getTrumpValue(const Card &card) const;
        int getRankValue(const Card &card) const;
        std::vector<Card> getPlaceableCards(const Suit &suit) const;
        std::vector<Card> getPlaceableCardsAscending(const Suit &suit) const;
        void sortByStrengthAscending(std::vector<Card> & cards) const;
    };
}
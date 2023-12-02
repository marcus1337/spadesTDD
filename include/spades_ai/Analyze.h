#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class Analyze
    {
        const Spades &spades;

    public:
        Analyze(const Spades &spades) : spades(spades)
        {
        }

        std::vector<Card> getPlayedRoundCards(const Seat &targetSeat) const
        {
            std::vector<Card> cards;
            for (const auto &[seat, card] : spades.getCurrentRoundCardSeatPairs())
            {
                if (targetSeat == seat)
                {
                    cards.push_back(card);
                }
            }
            return cards;
        }

        std::vector<Card> getRemainingNonSeatRoundCards(const Seat &seat) const
        {
            std::vector<Card> remainingOtherCards;
            for (const auto &otherSeat : SeatUtils::getOtherSeats(seat))
            {
                for (const auto &card : spades.getHand(otherSeat))
                {
                    remainingOtherCards.push_back(card);
                }
            }
            return remainingOtherCards;
        }

        std::set<Suit> getUnfollowedEffectiveLeadSuits(const Seat &seat) const
        {
            std::set<Suit> voidSuits;
            const auto cardSeatPairs = spades.getCurrentRoundCardSeatPairs();
            for (int i = 0; i < cardSeatPairs.size(); i++)
            {
                int trickIndex = i / NUM_SEATS;
                const auto leadCard = cardSeatPairs[trickIndex].second;
                const auto leadSeat = cardSeatPairs[trickIndex].first;
                const auto currentSeat = cardSeatPairs[i].first;
                const auto currentCard = cardSeatPairs[i].second;
                const bool unfollowedLead = spades.getEffectiveSuit(leadCard) != spades.getEffectiveSuit(currentCard);
                if (unfollowedLead && currentSeat == seat)
                {
                    voidSuits.insert(spades.getEffectiveSuit(leadCard));
                }
            }
            return voidSuits;
        }
    };
}
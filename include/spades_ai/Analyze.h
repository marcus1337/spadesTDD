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

        bool isEffectiveSuitInOtherHand(const Seat &perspectiveSeat, const Suit &suit) const
        {
            for (const auto &seat : SeatUtils::getOtherSeats(perspectiveSeat))
            {
                for (const auto &card : spades.getHand(seat))
                {
                    if (spades.getEffectiveSuit(card) == suit)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool isEffectiveSuitInAnyHand(const Suit &suit) const
        {
            for (const auto &seat : SeatUtils::getSeats())
            {
                for (const auto &card : spades.getHand(seat))
                {
                    if (spades.getEffectiveSuit(card) == suit)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        std::set<Suit> getSelfEffectiveSuits(const Seat &self) const
        {
            std::set<Suit> suits;
            for (const auto &card : spades.getHand(self))
            {
                suits.insert(spades.getEffectiveSuit(card));
            }
            return suits;
        }

        std::set<Suit> getSelfEffectiveVoidSuits(const Seat &self) const
        {
            std::set<Suit> voidSuits{Suit::SPADE, Suit::CLOVER, Suit::DIAMOND, Suit::HEART};
            for (const auto &suit : getSelfEffectiveSuits(self))
            {
                voidSuits.erase(suit);
            }
            return voidSuits;
        }

        std::set<Suit> getUnfollowedEffectiveLeadSuits(const Seat &targetSeat) const
        {
            std::set<Suit> voidSuits;
            const auto cardSeatPairs = spades.getCurrentRoundCardSeatPairs();
            for (int i = 0; i < cardSeatPairs.size(); i += NUM_SEATS)
            {
                for (int j = i + 1; j < i + NUM_SEATS && j < cardSeatPairs.size(); j++)
                {
                    const auto leadPair = cardSeatPairs[i];
                    const auto leadSuit = spades.getEffectiveSuit(leadPair.second);
                    const auto leadSeat = leadPair.first;

                    const auto followPair = cardSeatPairs[j];
                    const auto followSuit = spades.getEffectiveSuit(followPair.second);
                    const auto followSeat = followPair.first;

                    if (leadSuit != followSuit && followSeat == targetSeat)
                    {
                        voidSuits.insert(leadSuit);
                    }
                }
            }
            return voidSuits;
        }

        std::set<Suit> getVoidEffectiveSuits(const Seat &perspectiveSeat, const Seat &targetSeat) const
        {
            assert(perspectiveSeat != targetSeat);
            std::set<Suit> voidSuits = getUnfollowedEffectiveLeadSuits(targetSeat);
            for (const auto &suit : Card::getSuits())
            {
                if (!isEffectiveSuitInOtherHand(perspectiveSeat, suit))
                {
                    voidSuits.insert(suit);
                }
            }
            return voidSuits;
        }

        std::set<Suit> getEffectiveSuitsFromElimination(const Seat &perspectiveSeat, const Seat &targetSeat) const
        {
            const auto otherSeats = SeatUtils::getOtherSeats({perspectiveSeat, targetSeat});
            const auto voidTargetSuits = getVoidEffectiveSuits(perspectiveSeat, targetSeat);
            const auto voidSuits1 = getVoidEffectiveSuits(perspectiveSeat, otherSeats[0]);
            const auto voidSuits2 = getVoidEffectiveSuits(perspectiveSeat, otherSeats[1]);

            std::set<Suit> knownSuits;
            for (const auto &suit : Card::getSuits())
            {
                if (!voidTargetSuits.contains(suit) && voidSuits1.contains(suit) && voidSuits2.contains(suit))
                {
                    knownSuits.insert(suit);
                }
            }
            return knownSuits;
        }
    };
}
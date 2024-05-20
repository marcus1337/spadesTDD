#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class SuitTally
    {
    public:
        SuitTally(const Spades &spades) : numStartSuits{0}, numPlacedSuits{0}, numHandSuits{0}
        {
            setNumStartSuits(spades);
            setNumHandSuits(spades);
            setNumPlacedSuits(spades);
        }

        int getRemainingSuits(const Suit &suit) const
        {
            return numStartSuits[(unsigned int)suit] - numPlacedSuits[(unsigned int)suit];
        }

        bool otherSeatHasSuit(const Suit &suit) const
        {
            return getRemainingSuits(suit) != numHandSuits[(unsigned int)suit];
        }

    private:
        void setNumPlacedSuits(const Spades &spades)
        {
            for (const auto &[seat, card] : spades.getCurrentRoundCardSeatPairs())
            {
                numPlacedSuits[(unsigned int)spades.getEffectiveSuit(card)]++;
            }
        }

        void setNumHandSuits(const Spades &spades)
        {
            for (const auto &card : spades.getHand(spades.getTurnSeat()))
            {
                numHandSuits[(unsigned int)spades.getEffectiveSuit(card)]++;
            }
        }

        void setNumStartSuits(const Spades &spades)
        {
            setDefaultNumStartSuits();
            numStartSuits[(unsigned int)Suit::SPADE] = spades.getTrumpCardsDescending().size();
            subtractExcludedCards(spades);
        }

        void subtractExcludedCards(const Spades &spades)
        {
            for (const auto &exCard : spades.getExcludedCards())
            {
                for (const auto &suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART})
                {
                    if (exCard.is(suit))
                    {
                        numStartSuits[(unsigned int)suit]--;
                    }
                }
            }
        }

        void setDefaultNumStartSuits()
        {
            for (const auto &suit : Card::getSuits())
            {
                const auto suitIndex = (unsigned int)suit;
                numStartSuits[suitIndex] = 13;
            }
        }

        std::array<int, 4> numStartSuits;
        std::array<int, 4> numPlacedSuits;
        std::array<int, 4> numHandSuits;
    };

    class SuitSupplyObservation
    {
    public:
        SuitSupplyObservation(const Spades &spades);
        const std::array<float, 4 * (NUM_SEATS - 1)> &getValues() const;

    private:
        std::array<float, 4 * (NUM_SEATS - 1)> values;
        void setKnownAbsentSuits(const Spades &spades);
        void setKnownAbsentSuits(const Spades &spades, const Seat &otherSeat);
        std::size_t getValueIndex(unsigned int seatIndex, const Suit &suit) const;
        SuitTally suitTally;
        const bool spadesBroken = false;
    };
}
#pragma once
#include "spades_ai/logic/AIPlacer.h"
#include "spades/data/PortableRandom.h"

// Common sense decisions when having made the zero-bid.

namespace spd
{
    class ZeroAIPlacer : public AIPlacer
    {

        Card getFirstRoundCard(const Spades &spades, const Analyze &analyze) const
        {
            auto placeableCards = spades.getPlaceableCards();
            analyze.sortByStrengthAscending(placeableCards);
            std::vector<Card> fewestRemaining = analyze.getPlaceableCardsAscending(Suit::CLOVER);
            for (const auto &suit : {Suit::DIAMOND, Suit::HEART})
            {
                const auto &suitCards = analyze.getPlaceableCardsAscending(suit);
                if (suitCards.size() < fewestRemaining.size())
                {
                    fewestRemaining = suitCards;
                }
            }
            return fewestRemaining.front();
        }

        Card getAnyCard(const Spades &spades) const
        {
            return spades.getPlaceableCards().front();
        }

    public:
        ZeroAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) override
        {
            Analyze analyze(spades);
            const auto &placedCardSeats = spades.getCurrentRoundCardSeatPairs();
            if (placedCardSeats.empty())
            {
                return getFirstRoundCard(spades, analyze);
            }
            
            return getAnyCard(spades);
        }
    };
}
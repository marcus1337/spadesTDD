#pragma once
#include "spades_ai/logic/AIPlacer.h"
#include "spades/data/PortableRandom.h"

// Common sense decisions when having made the zero-bid.

namespace spd
{
    class ZeroAIPlacer : public AIPlacer
    {

        std::optional<Card> getLowestWinCard(const Spades &spades, const Analyze &analyze) const
        {
            auto placeableCards = spades.getPlaceableCards();
            analyze.sortByStrengthAscending(placeableCards);
            for (const auto &card : placeableCards)
            {
                if (spades.isTopCardIfPlaced(card))
                {
                    return card;
                }
            }
            return std::nullopt;
        }
        std::optional<Card> getHighesLoseCard(const Spades &spades, const Analyze &analyze) const
        {
            auto placeableCards = spades.getPlaceableCards();
            analyze.sortByStrengthAscending(placeableCards);
            std::reverse(placeableCards.begin(), placeableCards.end());
            for (const auto &card : placeableCards)
            {
                if (!spades.isTopCardIfPlaced(card))
                {
                    return card;
                }
            }
            return std::nullopt;
        }

        Card getLowestCardFromLeastOwnedSuit(const Spades &spades, const Analyze &analyze) const
        {
            std::vector<Card> fewestRemaining = analyze.getPlaceableCardsAscending(Suit::CLOVER);
            for (const auto &suit : {Suit::DIAMOND, Suit::HEART})
            {
                const auto &suitCards = analyze.getPlaceableCardsAscending(suit);
                if (suitCards.size() < fewestRemaining.size())
                {
                    fewestRemaining = suitCards;
                }
            }
            if (fewestRemaining.empty())
            {
                auto placeableCards = spades.getPlaceableCards();
                analyze.sortByStrengthAscending(placeableCards);
                return placeableCards.front();
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
            if (spades.getCurrentRoundCardSeatPairs().empty())
            {
                return getLowestCardFromLeastOwnedSuit(spades, analyze);
            }
            else
            {
                const auto &highestLose = getHighesLoseCard(spades, analyze);
                if (highestLose.has_value())
                {
                    return highestLose.value();
                }
                else
                {
                    return getLowestWinCard(spades, analyze).value();
                }
            }
        }
    };
}
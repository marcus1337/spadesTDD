#pragma once
#include "spades_ai/logic/AIPlacer.h"
#include "spades/data/PortableRandom.h"
namespace spd
{
    class AIAction
    {
    public:
        AIAction(const Spades &spades) : spades(spades), analyze(analyze)
        {
        }

        std::optional<Card> getLowestWinCard() const
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
        std::optional<Card> getHighesLoseCard() const
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

        Card getLowestCardFromLeastOwnedSuit() const
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

        Card getAnyCard() const
        {
            return spades.getPlaceableCards().front();
        }

    private:
        const Spades &spades;
        Analyze analyze;
    };
}

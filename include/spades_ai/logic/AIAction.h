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

        bool isFirst() const
        {
            return spades.getCurrentTrickCardSeatPairs().empty();
        }

        bool isLast() const
        {
            return spades.getCurrentTrickCardSeatPairs().size() == SeatUtils::getSeats().size() - 1;
        }

        Card getStrongestPlaceableCard() const
        {
            auto placeableCards = spades.getPlaceableCards();
            analyze.sortByStrengthAscending(placeableCards);
            return placeableCards.back();
        }
        Card getWeakestPlaceableCard() const
        {
            auto placeableCards = spades.getPlaceableCards();
            analyze.sortByStrengthAscending(placeableCards);
            return placeableCards.front();
        }

        Card getTakeTricksCardLast() const
        {
            if (const auto &winCardOpt = getLowestWinCard(); winCardOpt.has_value())
            {
                return winCardOpt.value();
            }
            else
            {
                return getWeakestPlaceableCard();
            }
        }

        Card getAvoidTricksCardLast() const
        {
            if (const auto &loseCardOpt = getHighesLoseCard(); loseCardOpt.has_value())
            {
                return loseCardOpt.value();
            }
            else
            {
                return getStrongestPlaceableCard();
            }
        }

        Card getTakeTricksCard() const
        {
            if (isFirst())
            {
                return getStrongestPlaceableCard();
            }
            else if (isLast())
            {
                return getTakeTricksCardLast();
            }
            return getAnyCard();
        }
        Card getAvoidTricksCard() const // place highest cards if must win, place highest lose card if can lose, place low card if may lose
        {
            if (isFirst())
            {
                return getWeakestPlaceableCard();
            }
            else if (isLast())
            {
                return getAvoidTricksCardLast();
            }
            return getAnyCard();
        }

    private:
        const Spades &spades;
        Analyze analyze;
    };
}

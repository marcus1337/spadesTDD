#include "spades_ai/AIAction.h"

using namespace spd;

AIAction::AIAction(const Spades &spades) : spades(spades)
{
}

std::vector<Card> AIAction::getPlaceableWinCards() const
{
    std::vector<Card> winCards;
    for (const auto &card : spades.getPlaceableCards())
    {
        if (spades.isTopCardIfPlaced(card))
        {
            winCards.push_back(card);
        }
    }
    return winCards;
}

std::vector<Card> AIAction::getPlaceableLoseCards() const
{
    std::vector<Card> loseCards;
    for (const auto &card : spades.getPlaceableCards())
    {
        if (!spades.isTopCardIfPlaced(card))
        {
            loseCards.push_back(card);
        }
    }
    return loseCards;
}

std::vector<Card> AIAction::copyCardsOrderedByStrengthDescending(const std::vector<Card> &cards) const
{
    std::vector<Card> copiedCards = cards;
    std::sort(copiedCards.begin(), copiedCards.end(), [&](const Card &c1, const Card &c2)
              { return spades.getCardStrengthRelativeToCurrentTrick(c1) > spades.getCardStrengthRelativeToCurrentTrick(c2); });
    return copiedCards;
}

Card AIAction::getRandomCard()
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}

std::optional<Card> AIAction::getHighestWinCard() const
{
    const auto winCards = copyCardsOrderedByStrengthDescending(getPlaceableWinCards());
    if (!winCards.empty())
    {
        return std::make_optional(winCards.front());
    }
    return std::nullopt;
}
std::optional<Card> AIAction::getLowestWinCard() const
{
    const auto winCards = copyCardsOrderedByStrengthDescending(getPlaceableWinCards());
    if (!winCards.empty())
    {
        return std::make_optional(winCards.back());
    }
    return std::nullopt;
}
std::optional<Card> AIAction::getHighestLoseCard() const
{
    const auto loseCards = copyCardsOrderedByStrengthDescending(getPlaceableLoseCards());
    if (!loseCards.empty())
    {
        return std::make_optional(loseCards.front());
    }
    return std::nullopt;
}
std::optional<Card> AIAction::getLowestLoseCard() const
{
    const auto loseCards = copyCardsOrderedByStrengthDescending(getPlaceableLoseCards());
    if (!loseCards.empty())
    {
        return std::make_optional(loseCards.back());
    }
    return std::nullopt;
}

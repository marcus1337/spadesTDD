#include "spades_ai/AIAction.h"

using namespace spd;

AIAction::AIAction(const Spades &spades) : spades(spades)
{
}

Card AIAction::getRandomCard()
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}
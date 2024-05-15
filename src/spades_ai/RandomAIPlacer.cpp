#include "spades_ai/placer/RandomAIPlacer.h"

using namespace spd;

Card RandomAIPlacer::getPlacement(const Spades &spades)
{
    return getRandomCard(spades);
}

Card RandomAIPlacer::getRandomCard(const Spades &spades)
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}

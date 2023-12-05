#include "spades_ai/RandomAIPlacer.h"

using namespace spd;

AIDifficulty RandomAIPlacer::getDifficulty() const
{
    return AIDifficulty::EASY;
}
Card RandomAIPlacer::getPlacement(const Spades &spades)
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}
std::string RandomAIPlacer::getName() const
{
    return "Random";
}
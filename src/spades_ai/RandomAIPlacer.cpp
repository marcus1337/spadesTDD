#include "spades_ai/RandomAIPlacer.h"
#include "spades_ai/AIAction.h"

using namespace spd;

AIDifficulty RandomAIPlacer::getDifficulty() const
{
    return AIDifficulty::EASY;
}
Card RandomAIPlacer::getPlacement(const Spades &spades)
{
    return AIAction(spades).getRandomCard();
}
std::string RandomAIPlacer::getName() const
{
    return "Random";
}
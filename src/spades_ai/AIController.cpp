#include "spades_ai/AIController.h"

using namespace spd;

AIPlacer &AIController::getPlacer()
{
    if (strategy == AIStrategy::RANDOM)
    {
        return randAI;
    }
    else
    {
        return defaultPlacer; // NORMAL AIStrategy
    }
}

AIController::AIController(std::shared_ptr<Spades> spades) : spades(spades)
{
}

void AIController::setSpades(std::shared_ptr<Spades> spades)
{
    this->spades = spades;
}

AIStrategy AIController::getStrategy() const
{
    return strategy;
}

void AIController::setStrategy(const AIStrategy &strategy)
{
    this->strategy = strategy;
}

int AIController::getBid() const
{
    assert(spades != nullptr);
    if (spades)
    {
        return AIBid(*spades.get()).getBid();
    }
    else
    {
        return -1;
    }
}

Card AIController::getPlacement()
{
    assert(spades != nullptr);
    return getPlacer().getPlacement(*spades.get());
}

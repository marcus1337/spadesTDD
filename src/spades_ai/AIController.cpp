#include "spades_ai/AIController.h"

using namespace spd;

AIPlacer &AIController::getPlacer()
{
    if (netAIPlacers.contains(strategy))
    {
        return *netAIPlacers.at(strategy);
    }

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

bool AIController::setNetAI(const std::string &encoding, const AIStrategy &strategy)
{
    auto netPlacer = std::make_unique<NetAIPlacer>();
    if (netPlacer->deserialize(encoding))
    {
        netAIPlacers[strategy] = std::move(netPlacer);
        return true;
    }
    else
    {
        return false;
    }
}

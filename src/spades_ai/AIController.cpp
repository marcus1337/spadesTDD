#include "spades_ai/AIController.h"

using namespace spd;

AIPlacer &AIController::getNormalPlacer()
{
    const auto seat = spades->getTurnSeat();
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    const auto opp1 = SeatUtils::getLeftOpponentSeat(seat);
    const auto opp2 = SeatUtils::getRightOpponentSeat(seat);
    const auto numTakenTricks = spades->getNumberOfTakenTricksCurrentRound(seat);
    const auto numTakenTricksTeamPlayer = spades->getNumberOfTakenTricksCurrentRound(teamSeat);
    const auto bid = spades->getBidResult(seat).value();
    const auto teamBid = spades->getBidResult(teamSeat).value();
    const auto opp1Bid = spades->getBidResult(opp1).value();
    const auto opp2Bid = spades->getBidResult(opp2).value();
    const auto numTricksOpp1 = spades->getNumberOfTakenTricksCurrentRound(opp1);
    const auto numTricksOpp2 = spades->getNumberOfTakenTricksCurrentRound(opp2);
    const bool enemyIsDefendingNil = (opp1Bid == 0 && numTricksOpp1 == 0) || (opp2Bid == 0 && numTricksOpp2 == 0);

    if (bid == 0 && numTakenTricks == 0)
    {
        return zeroPlacer;
    }
    else if (teamBid == 0 && numTakenTricksTeamPlayer == 0)
    {
        return defendZeroPlacer;
    }
    else if (enemyIsDefendingNil)
    {
        return sabotageZeroPlacer;
    }
    else
    {
        return targetAIPlacer;
    }
}

AIPlacer &AIController::getPlacer()
{
    if (strategy == AIStrategy::RANDOM)
    {
        return randAI;
    }
    else
    {
        return getNormalPlacer(); // NORMAL AIStrategy
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

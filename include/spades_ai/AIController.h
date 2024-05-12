#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/logic/Analyze.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/NetAIPlacer.h"
#include "spades_ai/logic/RandomAIPlacer.h"
#include "spades_ai/logic/AIBid.h"
#include "spades_ai/logic/ZeroAIPlacer.h"
#include "spades_ai/logic/TargetAIPlacer.h"

#include <iostream>
#include <fstream>

namespace spd
{
    class AIController
    {
        std::shared_ptr<Spades> spades;
        RandomAIPlacer randAI;
        ZeroAIPlacer zeroPlacer;
        SabotageZeroAIPlacer sabotageZeroPlacer;
        DefendZeroAIPlacer defendZeroPlacer;
        TargetAIPlacer targetAIPlacer;
        AIStrategy strategy = AIStrategy::ATTACK;

        AIPlacer &getPlacer()
        {
            if (strategy == AIStrategy::RANDOM)
            {
                return randAI;
            }

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

    public:
        AIController(std::shared_ptr<Spades> spades) : spades(spades)
        {
        }

        void setSpades(std::shared_ptr<Spades> spades)
        {
            this->spades = spades;
        }

        AIStrategy getStrategy() const
        {
            return strategy;
        }

        void setStrategy(const AIStrategy &strategy)
        {
            this->strategy = strategy;
        }

        int getBid() const
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

        Card getPlacement()
        {
            assert(spades != nullptr);
            return getPlacer().getPlacement(*spades.get());
        }
    };
}
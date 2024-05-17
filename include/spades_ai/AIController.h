#pragma once
#include "spades/Spades.h"
#include "spades_ai/logic/AIStrategy.h"
#include "spades_ai/logic/AIBid.h"

#include "spades_ai/placer/RandomAIPlacer.h"
#include "spades_ai/placer/ZeroAIPlacer.h"
#include "spades_ai/placer/TargetAIPlacer.h"
#include "spades_ai/placer/NetAIPlacer.h"

#include "spades_ai/placer/DefaultPlacer.h"

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
        AIStrategy strategy = AIStrategy::NORMAL;

        AIPlacer &getNormalPlacer();
        AIPlacer &getPlacer();

    public:
        AIController(std::shared_ptr<Spades> spades);
        void setSpades(std::shared_ptr<Spades> spades);
        AIStrategy getStrategy() const;
        void setStrategy(const AIStrategy &strategy);
        int getBid() const;
        Card getPlacement();
    };
}
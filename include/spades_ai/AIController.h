#pragma once
#include "spades/Spades.h"
#include "spades_ai/logic/AIStrategy.h"
#include "spades_ai/logic/AIBid.h"

#include "spades_ai/placer/DefaultPlacer.h"

namespace spd
{
    class AIController
    {
        std::shared_ptr<Spades> spades;
        RandomAIPlacer randAI;
        DefaultPlacer defaultPlacer;
        AIStrategy strategy = AIStrategy::NORMAL;
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
#pragma once
#include "spades_ai/logic/AIPlacer.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/AIAction.h"

// Common sense decisions when trying to take enough tricks to cover team bid but not more

namespace spd
{

    class TargetAIPlacer : public AIPlacer
    {
        virtual Card getPlacement(const Spades &spades) override
        {
            return AIAction(spades).getAnyCard();
        }
    };
}
#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/NetAIPlacer.h"

namespace spd
{
    class AIController
    {
        std::shared_ptr<Spades> spades;

    public:
        AIController(std::shared_ptr<Spades> spades) : spades(spades)
        {
        }
    };
}
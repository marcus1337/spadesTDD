#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/logic/Analyze.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/logic/NetAIPlacer.h"
#include "spades_ai/logic/RandomAIPlacer.h"
#include "spades_ai/logic/AIBid.h"

#include <iostream>
#include <fstream>

namespace spd
{
    class AIController
    {
        std::shared_ptr<Spades> spades;
        NetAIPlacer atkAI, defAI;
        RandomAIPlacer randAI;
        AIStrategy strategy = AIStrategy::ATTACK;

        std::string loadText(const std::string &filepath) const
        {
            std::ifstream file(filepath);
            if (file.is_open())
            {
                return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            }
            else
            {
                return "";
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

        bool load(const std::string &filepath, const AIStrategy &strategy)
        {
            switch (strategy)
            {
            case AIStrategy::ATTACK:
                return atkAI.deserialize(loadText(filepath));
            case AIStrategy::DEFEND:
                return defAI.deserialize(loadText(filepath));
            }
            return false;
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
            if (spades)
            {
                if (strategy == AIStrategy::RANDOM)
                {
                    return randAI.getPlacement(*spades.get());
                }
                else if (strategy == AIStrategy::ATTACK)
                {
                    return atkAI.getPlacement(*spades.get());
                }
                else
                {
                    return defAI.getPlacement(*spades.get());
                }
            }
            else
            {
                return Card();
            }
        }
    };
}
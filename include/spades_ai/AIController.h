#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"
#include "spades/data/PortableRandom.h"
#include "spades_ai/NetAIPlacer.h"

#include <iostream>
#include <fstream>

namespace spd
{
    class AIController
    {
        std::shared_ptr<Spades> spades;
        NetAIPlacer atkAI, defAI;

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
    };
}
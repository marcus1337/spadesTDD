#pragma once
#include "spades_ai/AIPlacer.h"
#include "spades/table/PortableRandom.h"

namespace spd
{
    class RandomAIPlacer : public AIPlacer
    {
        PortableRandom portableRandom;
    public:
        RandomAIPlacer(){

        }

        virtual AIDifficulty getDifficulty() const
        {
            return AIDifficulty::EASY;
        }
        virtual Card getPlacement(const Spades &spades)
        {
            
            //const auto placeableCards = spades
            return Card();
        }
        virtual std::string getName() const
        {
            return "Random";
        }
    };
}
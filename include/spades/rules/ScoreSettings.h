
#pragma once
#include <vector>
#include "spades/pch.h"

namespace spd
{
    class ScoreSettings
    {
    public:
        ScoreSettings() = default;
        void setWinScore(int score);
        void setLoseScore(int score);
        int getWinScore() const;
        int getLoseScore() const;

        int getBags(int totalBags) const
        {
            return numBagsPerSet != 0 ? totalBags % numBagsPerSet : 0;
        }
        int getNumBagSets(int totalBags) const
        {
            return numBagsPerSet != 0 ? totalBags / numBagsPerSet : 0;
        }
        int getNilValue() const
        {
            return nilValue;
        }
        int getNilBlindValue() const
        {
            return nilValue * 2;
        }
        int getPointsPerBag() const
        {
            return pointsPerBag;
        }

        int getPointsPerTrick() const
        {
            return trickValue;
        }

        int getPointsPerBlindTrick() const
        {
            return blindTrickValue;
        }

    private:
        int winScore = 250;
        int loseScore = -200;
        int trickValue = 10;
        int blindTrickValue = 10;
        int nilValue = 100;
        int bagSetPenalty = 100;
        int numBagsPerSet = 10;
        int pointsPerBag = 1;
    };

}

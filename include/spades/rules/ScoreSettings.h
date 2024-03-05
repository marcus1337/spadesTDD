
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

    private:
        int winScore = 250;
        int loseScore = -200;
    };

}

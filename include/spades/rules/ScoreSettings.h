
#pragma once
#include <vector>
#include "spades/pch.h"
#include "spades/data/Score.h"

namespace spd
{
    enum class ScoreResult
    {
        TEAM_A_WIN,
        TEAM_B_WIN,
        ONGOING
    };

    class ScoreSettings
    {
    public:
        ScoreSettings() = default;
        void setWinScore(int score);
        void setLoseScore(int score);
        int getWinScore() const;
        int getLoseScore() const;

        ScoreResult getResult(const Score& AScore, const Score& BScore) const;

    private:
        int winScore = 250;
        int loseScore = -200;
    };

}

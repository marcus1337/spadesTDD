#include "spades/rules/ScoreSettings.h"

using namespace spd;

void ScoreSettings::setWinScore(int score)
{
    winScore = score;
}
void ScoreSettings::setLoseScore(int score)
{
    loseScore = score;
}

int ScoreSettings::getWinScore() const
{
    return winScore;
}

int ScoreSettings::getLoseScore() const
{
    return loseScore;
}

ScoreResult ScoreSettings::getResult(const Score &AScore, const Score &BScore) const
{
    const auto aPoints = AScore.getPoints();
    const auto bPoints = BScore.getPoints();

    if (aPoints < getWinScore() && bPoints < getWinScore() &&
        aPoints > getLoseScore() && bPoints > getLoseScore())
    {
        return ScoreResult::ONGOING;
    }
    if (aPoints > bPoints)
    {
        return ScoreResult::TEAM_A_WIN;
    }
    else if (bPoints > aPoints)
    {
        return ScoreResult::TEAM_B_WIN;
    }
    else
    {
        return ScoreResult::ONGOING;
    }
}

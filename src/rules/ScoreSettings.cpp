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

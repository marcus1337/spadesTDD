#include "spades/rules/ScoreSettings.h"
#include "spades/data/memento/json.h"

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

int ScoreSettings::getBags(int totalBags) const
{
    return numBagsPerSet != 0 ? totalBags % numBagsPerSet : 0;
}
int ScoreSettings::getNumBagSets(int totalBags) const
{
    return numBagsPerSet != 0 ? totalBags / numBagsPerSet : 0;
}
int ScoreSettings::getNilValue() const
{
    return nilValue;
}
int ScoreSettings::getNilBlindValue() const
{
    return nilValue * 2;
}
int ScoreSettings::getPointsPerBag() const
{
    return pointsPerBag;
}

int ScoreSettings::getPointsPerTrick() const
{
    return trickValue;
}

int ScoreSettings::getPointsPerBlindTrick() const
{
    return blindTrickValue;
}

std::string ScoreSettings::serialize() const
{
    nlohmann::json j;
    j[WIN_KEY] = winScore;
    j[LOSE_KEY] = loseScore;
    j[TRICK_KEY] = trickValue;
    j[BLINDTRICK_KEY] = blindTrickValue;
    j[NIL_KEY] = nilValue;
    j[BAG_PENALTY_KEY] = bagSetPenalty;
    j[BAGS_PER_SET_KEY] = numBagsPerSet;
    j[POINTS_PER_BAG_KEY] = pointsPerBag;
    return j.dump();
}
bool ScoreSettings::deserialize(const std::string &encoding)
{
    const auto &winOpt = loadIntKey(encoding, WIN_KEY);
    const auto &loseOpt = loadIntKey(encoding, LOSE_KEY);
    const auto &trickOpt = loadIntKey(encoding, TRICK_KEY);
    const auto &blindTrickOpt = loadIntKey(encoding, BLINDTRICK_KEY);
    const auto &bagPenaltyOpt = loadIntKey(encoding, BAG_PENALTY_KEY);
    const auto &bagsPerSetOpt = loadIntKey(encoding, BAGS_PER_SET_KEY);
    const auto &pointsPerBagOpt = loadIntKey(encoding, POINTS_PER_BAG_KEY);

    for (const auto &opt : {winOpt, loseOpt, trickOpt, blindTrickOpt, bagPenaltyOpt, bagsPerSetOpt, pointsPerBagOpt})
    {
        if (!opt.has_value())
        {
            return false;
        }
    }

    winScore = winOpt.value();
    loseScore = loseOpt.value();
    trickValue = trickOpt.value();
    blindTrickValue = blindTrickOpt.value();
    bagSetPenalty = bagPenaltyOpt.value();
    numBagsPerSet = bagsPerSetOpt.value();
    pointsPerBag = pointsPerBagOpt.value();

    return true;
}

std::optional<int> ScoreSettings::loadIntKey(const std::string &encoding, const std::string &key) const
{
    std::optional<int> value;
    const auto &j = nlohmann::json::parse(encoding);
    if (j.contains(key) && j[key].is_number_integer())
    {
        value = j[key].get<int>();
        const int MAX_VALUE = 10000;
        value = std::clamp<int>(value.value(), -MAX_VALUE, MAX_VALUE);
    }
    return value;
}

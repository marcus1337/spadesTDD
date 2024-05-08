#include "spades/data/memento/SpadesMemento.h"
#include "spades/data/memento/json.h"
#include "spades/pch.h"

using namespace spd;

GameMode::GameMode()
{
    seed = Deck().getSeed();
}
std::string GameMode::serialize() const
{
    nlohmann::json j;
    j[BID_VAR_KEY] = (unsigned int)bidVarType;
    j[TRUMP_VAR_KEY] = (unsigned int)trumpVarType;
    j[SEED_KEY] = seed;
    return j.dump();
}
bool GameMode::deserialize(const std::string &data)
{
    seed = loadUIntKey(data, SEED_KEY).value_or(0);
    const auto bidVarValue = loadUIntKey(data, BID_VAR_KEY).value_or(0);
    const auto trumpVarValue = loadUIntKey(data, TRUMP_VAR_KEY).value_or(0);
    trumpVarType = (TrumpVariationType)std::clamp<unsigned int>(trumpVarValue, 0, (int)TrumpVariationType::LAST - 1);
    bidVarType = (BidVariationType)std::clamp<unsigned int>(bidVarValue, 0, (int)BidVariationType::LAST - 1);
    return true;
}

std::optional<unsigned int> GameMode::loadUIntKey(const std::string &encoding, const std::string &key)
{
    using namespace nlohmann;
    const auto &data = json::parse(encoding);
    if (data.contains(key) && data[key].is_number_unsigned())
    {
        return std::make_optional<unsigned int>(data[key].get<unsigned int>());
    }
    return std::nullopt;
}

//////////////

SpadesMemento::SpadesMemento(const SpadesHistory &history, const ScoreSettings &scoreSettings, const GameMode &gameMode)
{
    this->scoreSettings = scoreSettings;
    this->history = history;
    this->gameMode = gameMode;
}

SpadesMemento::SpadesMemento(const std::string &data)
{
    deserialize(data);
}

std::string SpadesMemento::serialize() const
{
    nlohmann::json j;
    j[GAME_MODE_KEY] = gameMode.serialize();
    j[SCORE_SETTINGS_KEY] = scoreSettings.serialize();
    j[HISTORY_KEY] = history.serialize();
    return j.dump();
}

bool SpadesMemento::deserialize(const std::string &data)
{
    using namespace nlohmann;
    const auto &j = json::parse(data);
    bool result = true;
    if (j.contains(GAME_MODE_KEY))
    {
        gameMode.deserialize(j[GAME_MODE_KEY]);
        result = false;
    }
    if (j.contains(SCORE_SETTINGS_KEY))
    {
        scoreSettings.deserialize(j[SCORE_SETTINGS_KEY]);
        result = false;
    }
    if (j.contains(HISTORY_KEY))
    {
        history.deserialize(j[HISTORY_KEY]);
        result = false;
    }
    return result;
}

//////////////

GameMode SpadesMemento::getGameMode() const
{
    return gameMode;
}

SpadesHistory SpadesMemento::getHistory() const
{
    return history;
}
ScoreSettings SpadesMemento::getScoreSettings() const
{
    return scoreSettings;
}

#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>
#include <optional>

#include "spades/rules/BidVariationType.h"
#include "spades/rules/TrumpVariationType.h"
#include "spades/data/Seat.h"
#include "spades/data/card/Card.h"
#include "spades/history/SpadesHistory.h"
#include "spades/rules/ScoreSettings.h"
#include "spades/rules/Deck.h"

namespace spd
{
    class GameMode : public Serializable
    {
        static constexpr const char *BID_VAR_KEY = "BidVar_key";
        static constexpr const char *TRUMP_VAR_KEY = "TrumpVar_key";
        static constexpr const char *SEED_KEY = "Seed_key";
        static std::optional<unsigned int> loadUIntKey(const std::string &encoding, const std::string &key);

    public:
        GameMode();
        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &data) override;
        TrumpVariationType trumpVarType = TrumpVariationType::ACE_HIGH;
        BidVariationType bidVarType = BidVariationType::DOUBLE_BLIND_NILL;
        unsigned int seed = 0;
    };

    class SpadesMemento : public Serializable
    {
        static constexpr const char *GAME_MODE_KEY = "Game_Mode_key";
        static constexpr const char *SCORE_SETTINGS_KEY = "Score_key";
        static constexpr const char *HISTORY_KEY = "History_key";

        GameMode gameMode;
        SpadesHistory history;
        ScoreSettings scoreSettings;

    public:
        SpadesMemento(const SpadesHistory &history, const ScoreSettings &scoreSettings, const GameMode &gameMode);
        SpadesMemento(const std::string &data);
        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &data) override;

        GameMode getGameMode() const;
        State getState() const;
        SpadesHistory getHistory() const;
        ScoreSettings getScoreSettings() const;
    };
}
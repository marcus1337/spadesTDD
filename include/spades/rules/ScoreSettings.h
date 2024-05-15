
#pragma once
#include <vector>
#include "spades/pch.h"
#include "spades/data/Serializable.h"

namespace spd
{
    class ScoreSettings : public Serializable
    {
    public:
        ScoreSettings() = default;
        void setWinScore(int score);
        int getWinScore() const;
        void setLoseScore(int score);
        int getLoseScore() const;

        void setPointsPerBag(int value);
        void setPointsPerTrick(int value);
        void setBagSetPenalty(int value);

        int getNilValue() const;
        int getNilBlindValue() const;
        int getPointsPerBag() const;
        int getPointsPerTrick() const;
        int getPointsPerBlindTrick() const;
        int getBagSetPenalty() const;
        void setBlindBonus(bool value);

        int getBags(int totalBags) const;
        int getNumBagSets(int totalBags) const;
        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &encoding) override;

    private:
        int winScore = 250;
        int loseScore = -200;
        int trickValue = 10;
        int nilValue = 100;
        int bagSetPenalty = 100;
        int numBagsPerSet = 10;
        int pointsPerBag = 1;
        bool blindBonus = false;

        static constexpr const char *WIN_KEY = "WIN_KEY";
        static constexpr const char *LOSE_KEY = "LOSE_KEY";
        static constexpr const char *TRICK_KEY = "TRICK_KEY";
        static constexpr const char *NIL_KEY = "NIL_KEY";
        static constexpr const char *BAG_PENALTY_KEY = "BAG_PENALTY_KEY";
        static constexpr const char *BAGS_PER_SET_KEY = "BAGS_PER_SET_KEY";
        static constexpr const char *POINTS_PER_BAG_KEY = "POINTS_PER_BAG_KEY";

        std::optional<int> loadIntKey(const std::string &encoding, const std::string &key) const;
    };

}

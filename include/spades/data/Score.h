#pragma once
#include <vector>
#include "spades/data/Seat.h"
#include <set>
#include "spades/data/Bid.h"
#include "spades/rules/Constants.h"
#include "spades/rules/ScoreSettings.h"

namespace spd
{
    struct SeatRoundResult
    {
        int tricks = 0;
        int bid = 0;
        std::set<BidOption> bidOptions;
        SeatRoundResult(const Seat &seat, int round, const std::vector<std::vector<Seat>> &completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> &completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>> &completedRoundBidOptions)
        {
            const auto &trickTakers = completedRoundTrickTakers[round];
            const auto &bids = completedRoundBids[round];
            const auto &bidOptionSeatPairs = completedRoundBidOptions[round];
            bid = getBid(seat, bids);
            tricks = std::count(trickTakers.begin(), trickTakers.end(), seat);
            for (const auto &pair : bidOptionSeatPairs)
            {
                if (pair.first == seat)
                {
                    bidOptions.emplace(pair.second);
                }
            }
        }

        int getCountedTricks() const
        {
            if (bid != 0)
            {
                return tricks;
            }
            else
            {
                return 0;
            }
        }
        int getBid(const Seat &seat, const std::array<std::pair<Seat, int>, NUM_SEATS> &bids) const
        {
            auto it = std::find_if(bids.begin(), bids.end(), [&seat](const auto &bid)
                                   { return bid.first == seat; });
            assert(it != bids.end());
            return (it != bids.end()) ? it->second : 0;
        }

        int getNilScore(const ScoreSettings &scoreSettings) const
        {
            int score = 0;
            if (bid == 0 && bidOptions.contains(BidOption::SHOW_HAND))
            {
                score = scoreSettings.getNilValue();
            }
            else if (bid == 0)
            {
                score = scoreSettings.getNilBlindValue();
            }
            return tricks == 0 ? score : -score;
        }

        int getPointsPerTrick(const ScoreSettings &scoreSettings) const
        {
            return bidOptions.contains(BidOption::SHOW_HAND) ? scoreSettings.getPointsPerTrick() : scoreSettings.getPointsPerBlindTrick();
        }
    };

    enum class ScoreResult
    {
        TEAM_A_WIN,
        TEAM_B_WIN,
        ONGOING
    };

    class Score
    {
        const ScoreSettings scoreSettings;
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
        int getTotalBags() const;
        int getNumBagSets() const;
        int getRoundPoints(const SeatRoundResult &r1, const SeatRoundResult &r2) const;
        int getRoundBags(const SeatRoundResult &r1, const SeatRoundResult &r2) const;
        int getRoundTrickPoints(const SeatRoundResult &r1, const SeatRoundResult &r2) const;

    public:
        Score(const ScoreSettings &scoreSettings, const std::pair<Seat, Seat> &team, const std::vector<std::vector<Seat>> &completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> &completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>> &completedRoundBidOptions);
        std::vector<int> getRoundBags() const;
        std::vector<int> getRoundPoints() const;
        int getBags() const;
        int getPoints() const;

        static ScoreResult getScoreResult(const ScoreSettings &scoreSettings, const Score &score1, const Score &score2);
    };

}
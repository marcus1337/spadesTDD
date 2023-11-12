#include "data/Score.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <numeric>

using namespace spd;

Score::Score(const std::pair<Seat, Seat> &team, const std::vector<std::vector<Seat>> &completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat, int>, SeatUtils::numSeats>> &completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>> &completedRoundBidOptions)
{
    assert(completedRoundTrickTakers.size() == completedRoundBids.size());
    assert(completedRoundTrickTakers.size() == completedRoundBidOptions.size());
    const int numRounds = completedRoundTrickTakers.size();
    for (int round = 0; round < numRounds; round++)
    {

        auto trickTakers = completedRoundTrickTakers[round];
        auto bids = completedRoundBids[round];
        auto bidOptions = completedRoundBidOptions[round];

        int numTricks1 = getNumTakenTricks(team.first, trickTakers);
        int numTricks2 = getNumTakenTricks(team.second, trickTakers);
        int bid1 = getBid(team.first, bids);
        int bid2 = getBid(team.second, bids);
        bool failedNil1 = numTricks1 > 0 && bid1 == 0;
        bool failedNil2 = numTricks2 > 0 && bid2 == 0;
        bool nil1 = numTricks1 == 0 && bid1 == 0;
        bool nil2 = numTricks2 == 0 && bid2 == 0;
        int nilMultiplier1 = bidOptions.contains({team.first, BidOption::SHOW_HAND}) ? 1 : 2;
        int nilMultiplier2 = bidOptions.contains({team.second, BidOption::SHOW_HAND}) ? 1 : 2;

        int teamTricks = 0;
        if (bid1 != 0)
        {
            teamTricks += numTricks1;
        }
        if (bid2 != 0)
        {
            teamTricks += numTricks2;
        }

        int points = 0;
        const int trickValue = 10;
        const int nilValue = 100;
        if (teamTricks >= bid1 + bid2)
        {
            points += teamTricks * trickValue;
            points += teamTricks - bid1 - bid2;
        }
        else
        {
            points -= teamTricks * trickValue;
        }
        if (nil1)
        {
            points += nilValue * nilMultiplier1;
        }
        else if (failedNil1)
        {
            points -= nilValue * nilMultiplier1;
        }
        if (nil2)
        {
            points += nilValue * nilMultiplier2;
        }
        else if (failedNil2)
        {
            points -= nilValue * nilMultiplier2;
        }

        int bags = std::max<int>(0, teamTricks - bid1 - bid2);
        roundPoints.push_back(points);
        roundBags.push_back(bags);
    }
}

int Score::getNumTakenTricks(const Seat &seat, const std::vector<Seat> &trickTakers) const
{
    return std::count(trickTakers.begin(), trickTakers.end(), seat);
}

int Score::getBid(const Seat &seat, const std::array<std::pair<Seat, int>, SeatUtils::numSeats> &bids) const
{
    for (const auto bid : bids)
    {
        if (bid.first == seat)
        {
            return bid.second;
        }
    }
    return 0;
}

std::vector<int> Score::getRoundBags() const
{
    return roundBags;
}

std::vector<int> Score::getRoundPoints() const
{
    return roundPoints;
}

int Score::getPoints() const
{
    const int bagSetPenalty = 100;
    const int numBagsPerBagSet = 10;
    const int numBags = std::accumulate(roundBags.begin(), roundBags.end(), 0);
    const int points = std::accumulate(roundPoints.begin(), roundPoints.end(), 0);
    return points - (numBags / numBagsPerBagSet) * bagSetPenalty;
}
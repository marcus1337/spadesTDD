#include "data/Score.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <numeric>
#include <iterator>

using namespace spd;

struct SeatScore
{
    int numTricks;
    int bid;
    bool nilFail;
    bool nilSuccess;
    int nilMultiplier;
    int nilScore = 0;

    SeatScore(const Seat &seat, const std::vector<Seat> &trickTakers, const std::array<std::pair<Seat, int>, SeatUtils::numSeats> &roundBids, const std::set<std::pair<Seat, BidOption>> &roundBidOptions)
    {
        numTricks = std::count(trickTakers.begin(), trickTakers.end(), seat);
        bid = getBid(seat, roundBids);
        nilFail = numTricks > 0 && bid == 0;
        nilSuccess = numTricks == 0 && bid == 0;
        nilMultiplier = roundBidOptions.contains({seat, BidOption::SHOW_HAND}) ? 1 : 2;

        const int nilValue = 100;
        if (nilSuccess)
        {
            nilScore = nilValue * nilMultiplier;
        }
        else if (nilFail)
        {
            nilScore = -nilValue * nilMultiplier;
        }
    }

    int getBid(const Seat &seat, const std::array<std::pair<Seat, int>, SeatUtils::numSeats> &bids) const
    {
        auto it = std::find_if(bids.begin(), bids.end(), [&seat](const auto &bid)
                               { return bid.first == seat; });
        assert(it != bids.end());
        return (it != bids.end()) ? it->second : 0;
    }

    int getCountedTricks() const
    {
        if (bid != 0)
        {
            return numTricks;
        }
        else
        {
            return 0;
        }
    }

    int getTeamTricks(const SeatScore &teamScore) const
    {
        return getCountedTricks() + teamScore.getCountedTricks();
    }

    int getTeamBags(const SeatScore &teamScore) const
    {
        return std::max(0, getTeamTricks(teamScore) - bid - teamScore.bid);
    }

    int getRoundPoints(const SeatScore &teamScore) const
    {
        int nilScore = nilScore + teamScore.nilScore;
        int trickScore = 0;
        const int trickValue = 10;
        const int teamTricks = getTeamTricks(teamScore);
        if (teamTricks >= bid + teamScore.bid)
        {
            trickScore += teamTricks * trickValue;
            trickScore += getTeamBags(teamScore);
        }
        return nilScore + trickScore;
    }
};

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
        auto seatScore1 = SeatScore(team.first, trickTakers, bids, bidOptions);
        auto seatScore2 = SeatScore(team.second, trickTakers, bids, bidOptions);
        roundPoints.push_back(seatScore1.getRoundPoints(seatScore2));
        roundBags.push_back(seatScore1.getTeamBags(seatScore2));
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
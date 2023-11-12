#pragma once
#include <vector>
#include <array>
#include "data/Seat.h"
#include <set>
#include "data/Bid.h"

namespace spd
{
    class Score
    {
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
    public:
        Score(const std::array<Seat, 2>& team, const std::vector<std::vector<Seat>>& completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat,int>, SeatUtils::numSeats>>& completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>>& completedRoundBidOptions);
        std::vector<int> getRoundBags() const;
        std::vector<int> getRoundPoints() const;
        int getPoints() const;
    };

}
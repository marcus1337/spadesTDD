#pragma once
#include <vector>
#include "data/Seat.h"
#include <set>
#include "data/Bid.h"

namespace spd
{
    class Score
    {
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
        int getNumTakenTricks(const Seat& seat, const std::vector<Seat>& trickTakers) const;
        int getBid(const Seat& seat, const std::array<std::pair<Seat, int>, SeatUtils::numSeats>& bids) const;
    public:
        Score(const std::pair<Seat,Seat>& team, const std::vector<std::vector<Seat>>& completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat,int>, SeatUtils::numSeats>>& completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>>& completedRoundBidOptions);
        std::vector<int> getRoundBags() const;
        std::vector<int> getRoundPoints() const;
        int getPoints() const;
    };

}
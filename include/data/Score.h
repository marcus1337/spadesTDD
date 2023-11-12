#pragma once
#include <vector>
#include "data/Seat.h"
#include <set>
#include "data/Bid.h"

namespace spd
{
    class Score
    {
        static constexpr int numBagsPerSet = 10;
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
        int getTotalBags() const;
        int getNumBagSets() const;
    public:
        Score(const std::pair<Seat,Seat>& team, const std::vector<std::vector<Seat>>& completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat,int>, SeatUtils::numSeats>>& completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>>& completedRoundBidOptions);
        std::vector<int> getRoundBags() const;
        std::vector<int> getRoundPoints() const;
        int getBags() const;
        int getPoints() const;
    };

}
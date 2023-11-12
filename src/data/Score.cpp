#include "data/Score.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <numeric>

using namespace spd;

Score::Score(const std::array<Seat, 2>& team, const std::vector<std::vector<Seat>>& completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat,int>, SeatUtils::numSeats>>& completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>>& completedRoundBidOptions){
    assert(completedRoundTrickTakers.size() == completedRoundBids.size());
    assert(completedRoundTrickTakers.size() == completedRoundBidOptions.size());
}

std::vector<int> Score::getRoundBags() const{
    return {};
}

std::vector<int> Score::getRoundPoints() const{
    return {};
}

int Score::getPoints() const{
    return 0;
}
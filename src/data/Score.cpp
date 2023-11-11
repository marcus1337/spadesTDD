#include "data/Score.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <numeric>

using namespace spd;

Score::Score(const std::array<Seat, 2> &team, const std::vector<Seat> &trickTakers, const State &state){

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
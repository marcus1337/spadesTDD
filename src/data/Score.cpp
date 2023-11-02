#include "data/Score.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <numeric>

using namespace spd;

int TeamScore::getTotalPoints() const
{
    return std::accumulate(roundPoints.begin(), roundPoints.end(), 0);
}
int TeamScore::getTotalBags() const
{
    return std::accumulate(roundBags.begin(), roundBags.end(), 0);
}
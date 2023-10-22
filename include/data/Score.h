#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <optional>
#include <numeric>

namespace spd
{
    struct TeamScore{
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
        int getTotalPoints() const {
            return std::accumulate(roundPoints.begin(), roundPoints.end(), 0);
        }
        int getTotalBags() const {
            return std::accumulate(roundBags.begin(), roundBags.end(), 0);
        }
    };

    struct Score{
        const TeamScore teamScore1;
        const TeamScore teamScore2;
    };

}
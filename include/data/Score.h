#pragma once
#include <vector>

namespace spd
{
    struct TeamScore{
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
        int getTotalPoints() const;
        int getTotalBags() const;
    };

    struct Score{
        const TeamScore teamScore1;
        const TeamScore teamScore2;
    };

}
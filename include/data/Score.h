#pragma once
#include <vector>
#include <array>
#include "data/Seat.h"
#include "data/State.h"

namespace spd
{
    class Score
    {
        std::vector<int> roundPoints;
        std::vector<int> roundBags;
    public:
        Score(const std::array<Seat, 2>& team, const std::vector<Seat>& trickTakers, const State& state);
        std::vector<int> getRoundBags() const;
        std::vector<int> getRoundPoints() const;
        int getPoints() const;
    };

}
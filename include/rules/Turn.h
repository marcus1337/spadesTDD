#pragma once
#include "data/State.h"

namespace spd
{
    class Turn
    {

    public:
        Seat getTurnSeat(const State &state) const;
    };

}
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"

namespace spd
{
    struct CardEvaluator {
        virtual int getValue(const Card& card) = 0;
    };

    struct BidRule {
        virtual std::vector<int> getPossibleBids() = 0;
    };

    class Spades
    {

    public:
        int getTestNumber() const;
    };
}
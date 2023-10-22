
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>

namespace spd
{
    struct Bid{
        int numTricks;
        bool blind;
        Bid(int numTricks, bool blind) : numTricks(){
        }
    };

    class BidVariation {
        public:
        virtual std::vector<Bid> getPossibleBids() = 0;
    };

    class DoubleBlindNil : public BidVariation{
        virtual std::vector<Bid> getPossibleBids() override {
            return {};
        }
    };
}

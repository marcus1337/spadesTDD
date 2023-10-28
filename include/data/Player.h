
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/card/Card.h"
#include <string>
#include <optional>
#include "data/Bid.h"

namespace spd
{
    enum class Seat{
        SOUTH = 0, WEST, NORTH, EAST
    };

    class Player{
        std::optional<int> bid;
        std::vector<Card> hand;
        public:
        void setBid(int bid){
            this->bid = std::optional<int>(bid);
        }
        bool hasBid() const {
            return bid.has_value();
        }
    };
}
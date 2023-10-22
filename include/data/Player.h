
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <optional>

namespace spd
{
    struct Bid{
        int tricks;
        bool blind;
    };

    struct Player{
        std::vector<Card> hand;
        std::optional<Bid> bid;
        bool blind;
        bool hasBid() const {
            return bid.has_value();
        }
    };

    struct Team{
        Player player1;
        Player player2; 
        bool hasBid() const {
            return player1.hasBid() && player2.hasBid();
        }
    };
}
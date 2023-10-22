
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <optional>
#include "data/Player.h"

namespace spd
{
    class BidVariation {
        protected:
        const int MAX_BID = 13;
        public:
        virtual std::vector<Bid> getPossibleBids(const Player& player, const Player& teamPlayer, const Team& playerTeam, const Team& enemyTeam) = 0;
        virtual ~BidVariation() = default;
    };
    class DoubleBlindNil : public BidVariation{
        virtual std::vector<Bid> getPossibleBids(const Player& player, const Player& teamPlayer, const Team& playerTeam, const Team& enemyTeam) override {
            const int teamBid = teamPlayer.hasBid() ? teamPlayer.bid.value().tricks : 0;
            std::vector<Bid> bids;
            for(int i = 0; i < MAX_BID-teamBid; i++)
                bids.push_back(Bid{.tricks=i,.blind=player.blind});
            return bids;
        }
    };
    class DoubleNil : public BidVariation{
        virtual std::vector<Bid> getPossibleBids(const Player& player, const Player& teamPlayer, const Team& playerTeam, const Team& enemyTeam) override {
            const int teamBid = teamPlayer.hasBid() ? teamPlayer.bid.value().tricks : 0;
            std::vector<Bid> bids;
            for(int i = 0; i < MAX_BID-teamBid; i++)
                bids.push_back(Bid{.tricks=i,.blind=false});
            return bids;
        }
    };
}

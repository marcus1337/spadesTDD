
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include "data/card/Card.h"
#include "data/Player.h"
#include <memory>
#include <map>
#include <cassert>
#include "data/State.h"

namespace spd
{
    class BidVariation {
        protected:
        const int MAX_BID = 13;
        public:
        virtual std::vector<int> getPossibleBids(const Seat& seat, const State& state) const = 0;
        virtual ~BidVariation() = default;
    };

    class DoubleBlindNil : public BidVariation{
        virtual std::vector<int> getPossibleBids(const Seat& seat, const State& state) const override {
            //const int teamBid = teamPlayer.hasBid() ? teamPlayer.bid.value().tricks : 0;
            const int teamBid = 0;
            std::vector<int> bids;
            for(int i = 0; i < MAX_BID-teamBid; i++)
                bids.push_back(i);
            return bids;
        }
    };
    class DoubleNil : public BidVariation{
        virtual std::vector<int> getPossibleBids(const Seat& seat, const State& state) const override {
            //const int teamBid = teamPlayer.hasBid() ? teamPlayer.bid.value().tricks : 0;
            const int teamBid = 0;
            std::vector<int> bids;
            for(int i = 0; i < MAX_BID-teamBid; i++)
                bids.push_back(i);
            return bids;
        }
    };

}

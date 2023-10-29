
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include "data/card/Card.h"
#include <memory>
#include <map>
#include <cassert>
#include "data/State.h"

namespace spd
{
    class BidVariation
    {
    protected:
        const int MAX_BID = 13;

    public:
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const = 0;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const = 0;
        virtual ~BidVariation() = default;
    };

    class DoubleBlindNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            int teamBid = 0;
            const auto teamSeat = SeatUtils::getTeamSeat(seat);
            if(state.hasBid(teamSeat)){
                teamBid += state.getBid(teamSeat);
            }
            std::vector<int> bids;
            for (int i = 0; i <= MAX_BID - teamBid; i++)
                bids.push_back(i);
            return bids;
        }

        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            if(!state.hasBidOption(seat, BidOption::SHOW_HAND))
            {
                return {BidOption::SHOW_HAND};
            }else{
                return {};
            }
        }
    };
    class DoubleNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            const int teamBid = 0;
            std::vector<int> bids;
            for (int i = 0; i <= MAX_BID - teamBid; i++)
                bids.push_back(i);
            return bids;
        }
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            return {};
        }
    };

}

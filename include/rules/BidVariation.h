
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
        std::vector<int> getAdjustedBidsBasedOnTeam(const Seat &seat, const State &state) const
        {
            int teamBid = 0;
            const auto teamSeat = SeatUtils::getTeamSeat(seat);
            if (state.hasBid(teamSeat))
            {
                teamBid += state.getBid(teamSeat);
            }
            std::vector<int> bids;
            for (int i = 0; i <= MAX_BID - teamBid; i++)
                bids.push_back(i);
            return bids;
        }

        std::vector<int> getBidEqualToSpades(const Seat &seat, const State &state) const
        {
            const auto &hand = state.getHand(seat);
            const int numSpades = std::count_if(hand.begin(), hand.end(),
                                                [](const auto &card)
                                                { return card.is(Suit::SPADE); });
            return {numSpades};
        }

    public:
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const = 0;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const = 0;
        virtual ~BidVariation() = default;
    };

    class DoubleBlindNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            return getAdjustedBidsBasedOnTeam(seat, state);
        }

        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            if (!state.hasBidOption(seat, BidOption::SHOW_HAND))
            {
                return {BidOption::SHOW_HAND};
            }
            else
            {
                return {};
            }
        }
    };
    class DoubleNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            return getAdjustedBidsBasedOnTeam(seat, state);
        }
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            return {};
        }
    };

    class Mirror : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            return getBidEqualToSpades(seat, state);
        }
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            return {};
        }
    };

    class Suicide : public BidVariation
    {
        std::vector<int> getPossibleNonNilBids() const
        {
            std::vector<int> bids;
            for (int i = 4; i <= MAX_BID; i++)
            {
                bids.push_back(i);
            }
            return bids;
        }

        std::vector<int> getPossibleFirstBids() const
        {
            std::vector<int> bids;
            bids.push_back(0);
            for (const int bid : getPossibleNonNilBids())
            {
                bids.push_back(bid);
            }
            return bids;
        }

        std::vector<int> getPossibleSecondaryBids(const Seat &seat, const State &state) const
        {
            std::vector<int> bids;
            const auto teamSeat = SeatUtils::getTeamSeat(seat);
            if (state.getBid(teamSeat) != 0)
            {
                bids.push_back(0);
                bids.push_back(state.getBid(teamSeat));
            }
            else
            {
                for (const int bid : getPossibleNonNilBids())
                {
                    bids.push_back(bid);
                }
            }
            return bids;
        }

    public:
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override
        {
            if (state.hasBid(SeatUtils::getTeamSeat(seat)))
                return getPossibleSecondaryBids(seat, state);
            else
                return getPossibleFirstBids();
        }

        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override
        {
            return {};
        }
    };

}

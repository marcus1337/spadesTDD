#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include <map>
#include <set>
#include "data/card/Card.h"

namespace spd
{
    class State
    {

        std::vector<int> getMadeRoundBids() const
        {
            std::vector<int> madeRoundBids;
            const int fromIndex = getRound() * SeatUtils::numSeats;
            const int toIndex = fromIndex + SeatUtils::numSeats;
            for (int i = fromIndex; i < toIndex && i < bids.size(); i++){
                madeRoundBids.push_back(bids[i]);
            }
            return madeRoundBids;
        }

        std::array<Seat, SeatUtils::numSeats> getRoundBidOrder() const {
            std::array<Seat, SeatUtils::numSeats> bidOrder{};
            const Seat startSeat = SeatUtils::getStartBidSeat(getRound());
            const auto seatOrder = SeatUtils::getSeatOrder(startSeat, SeatUtils::numSeats);
            std::copy(seatOrder.begin(), seatOrder.end(), bidOrder.begin());
            return bidOrder;
        }

        std::array<std::optional<std::pair<Seat, int>>, SeatUtils::numSeats> getRoundBids() const
        {
            std::array<std::optional<std::pair<Seat, int>>, SeatUtils::numSeats> roundBids{};
            const auto roundBidOrder = getRoundBidOrder();
            const auto madeRoundBids = getMadeRoundBids();
            for(int i = 0 ; i < madeRoundBids.size(); i++){
                roundBids[i] = std::make_optional(std::make_pair(roundBidOrder[i], madeRoundBids[i]));
            }
            return roundBids;
        }

    public:
        State() = default;

        void clear()
        {
            bids.clear();
        }

        int getRound() const
        {
            return playedCards.size() % 52;
        }

        bool isBidPhase() const
        {
            return bids.size() < (getRound() + 1) * SeatUtils::numSeats;
        }

        Seat getTurnSeat() const
        {
            if (isBidPhase())
            {
                int playerIndex = (bids.size() + getRound()) % SeatUtils::numSeats;
                return (Seat)playerIndex;
            }
            return Seat::SOUTH;
        }

        bool hasBidOption(const Seat &seat, const BidOption &bidOption) const
        {
            if (!roundBidOptions.contains(getRound()))
                return false;
            return roundBidOptions.at(getRound()).contains(std::make_pair(seat, bidOption));
        }

        void setBidOption(const Seat &seat, const BidOption &bidOption)
        {
            if (!roundBidOptions.contains(getRound()))
                roundBidOptions[getRound()] = std::set<std::pair<Seat, BidOption>>();
            roundBidOptions[getRound()].emplace(std::make_pair(seat, bidOption));
        }

        bool hasBid(const Seat &seat) const
        {
            if (!isBidPhase())
            {
                return true;
            }
            else
            {
                const Seat startBidSeat = SeatUtils::getStartBidSeat(getRound());
                const int numMadeBids = bids.size() % SeatUtils::numSeats;
                for (const auto &biddedSeat : SeatUtils::getSeatOrder(startBidSeat, numMadeBids))
                {
                    if (biddedSeat == seat)
                    {
                        return true;
                    }
                }
                return false;
            }
        }

        int getBid(const Seat &seat) const
        {
            for(const auto& roundBid : getRoundBids()){
                if(roundBid.has_value() && roundBid->first == seat){
                    return roundBid->second;
                }
            }
            return -1;
        }

        bool hasGameStarted() const
        {
            return !bids.empty();
        }

        std::vector<int> bids;
        std::vector<Card> playedCards;
        std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;
    };
}
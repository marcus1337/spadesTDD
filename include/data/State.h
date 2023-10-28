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

        bool hasBidOption(const Seat& seat, const BidOption& bidOption) const {
            if(!roundBidOptions.contains(getRound()))
                return false;
            return roundBidOptions.at(getRound()).contains(std::make_pair(seat, bidOption));
        }

        void setBidOption(const Seat& seat, const BidOption& bidOption){
            if(!roundBidOptions.contains(getRound()))
                roundBidOptions[getRound()] = std::set<std::pair<Seat,BidOption>>();
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
                for(const auto& biddedSeat : SeatUtils::getSeatOrder(startBidSeat, numMadeBids)){
                    if(biddedSeat == seat){
                        return true;
                    }
                }
                return false;
            }
        }

        bool hasGameStarted() const
        {
            return !bids.empty();
        }

        std::vector<int> bids;
        std::vector<Card> playedCards;
        std::map<int, std::set<std::pair<Seat,BidOption>>> roundBidOptions;
    };
}
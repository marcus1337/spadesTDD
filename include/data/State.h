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
    class State
    {

        Seat getStartBidder() const
        {
            return (Seat)(getRound() % 4);
        }

        std::vector<Seat> getSeats(const Seat& from, int numSeats) const {
            if(numSeats == 0){
                return {};
            }
            std::vector<Seat> seats = {from};
            numSeats--;
            std::vector<Seat> nextSeats = getNextSeats(from, numSeats);
            seats.insert(seats.end(), nextSeats.begin(), nextSeats.end());
            return seats;
        }

        std::vector<Seat> getNextSeats(const Seat &from, int numSteps) const
        {
            std::vector<Seat> seats;
            Seat seat = from;
            for(int i = 0 ; i < numSteps; i++){
                seat = getNextSeat(seat);
                seats.push_back(seat);
            }
            return seats;
        }

        Seat getNextSeat(const Seat &from) const
        {
            int playerIndex = ((int)from + 1) % 4;
            return (Seat)playerIndex;
        }

    public:
        State()
        {
        }

        void clear()
        {
            bids.clear();
        }

        int getRound() const
        {
            return (int)(bids.size() / 4);
        }

        bool isBidPhase() const
        {
            return bids.size() < (getRound() + 1) * 4;
        }

        Seat getTurnSeat() const
        {
            if (isBidPhase())
            {
                int playerIndex = (bids.size() + getRound()) % 4;
                return (Seat)playerIndex;
            }
            return Seat::SOUTH;
        }

        bool hasPlayerBid(const Seat &seat) const
        {
            if (!isBidPhase())
            {
                return true;
            }
            else
            {
                for(const auto& biddedSeat : getSeats(getStartBidder(), bids.size() % 4)){
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
    };
}
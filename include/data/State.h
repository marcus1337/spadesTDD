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
            return bids.size() < (getRound()+1) * 4;
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
                return false;
            }
            else
            {
                for (int i = 0; i < bids.size() % 4; i++)
                {
                    int seatIndex = ((int)getStartBidder() + i) % 4;
                    if ((Seat)seatIndex == seat)
                    {
                        return true;
                    }
                }
                return false;
            }
        }

        bool hasGameStarted() const {
            return !bids.empty();
        }

        std::vector<int> bids;
    };
}
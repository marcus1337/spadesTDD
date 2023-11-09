
#pragma once
#include <vector>
#include <array>
#include "data/Bid.h"

namespace spd
{
    enum class Seat
    {
        SOUTH = 0,
        WEST,
        NORTH,
        EAST
    };

    namespace SeatUtils
    {
        static constexpr int numSeats = 4;
        inline std::array<Seat, numSeats> getSeats()
        {
            return {Seat::SOUTH, Seat::WEST, Seat::NORTH, Seat::EAST};
        }

        inline Seat getTeamSeat(const Seat &from)
        {
            int playerIndex = ((int)from + 2) % 4;
            return (Seat)playerIndex;
        }

        inline Seat getStartBidSeat(int round)
        {
            return (Seat)(round % SeatUtils::numSeats);
        }

        inline Seat getNextSeat(const Seat &from)
        {
            int playerIndex = ((int)from + 1) % 4;
            return (Seat)playerIndex;
        }

        inline std::vector<Seat> getNextSeats(const Seat &from, int numSteps)
        {
            std::vector<Seat> seats;
            Seat seat = from;
            for (int i = 0; i < numSteps; i++)
            {
                seat = getNextSeat(seat);
                seats.push_back(seat);
            }
            return seats;
        }

        inline std::vector<Seat> getSeatOrder(const Seat &from, int numTurns)
        {
            std::vector<Seat> seatOrder;
            if (numTurns > 0)
            {
                seatOrder.push_back(from);
                numTurns--;
                std::vector<Seat> nextSeats = getNextSeats(from, numTurns);
                seatOrder.insert(seatOrder.end(), nextSeats.begin(), nextSeats.end());
            }
            return seatOrder;
        }
    }

}
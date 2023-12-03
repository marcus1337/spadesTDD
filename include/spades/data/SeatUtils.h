#pragma once
#include "spades/data/Seat.h"
#include "spades/rules/Constants.h"

namespace spd
{
    namespace SeatUtils
    {
        inline std::array<Seat, NUM_SEATS> getSeats()
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
            return (Seat)(round % NUM_SEATS);
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

        inline std::vector<Seat> getOtherSeats(const Seat &excludedSeat)
        {
            std::vector<Seat> otherSeats;
            otherSeats.push_back(getNextSeat(excludedSeat));
            otherSeats.push_back(getNextSeat(otherSeats.back()));
            otherSeats.push_back(getNextSeat(otherSeats.back()));
            return otherSeats;
        }

        inline std::vector<Seat> getOtherSeats(const std::set<Seat> &excludedSeats)
        {
            std::vector<Seat> otherSeats;
            for (const auto &seat : getSeats())
            {
                if (!excludedSeats.contains(seat))
                {
                    otherSeats.push_back(seat);
                }
            }
            return otherSeats;
        }
    }
}
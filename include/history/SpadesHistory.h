#pragma once
#include "history/SpadesCommand.h"
#include <memory>
#include <vector>

namespace spd
{
    class SpadesHistory
    {
        std::vector<std::unique_ptr<SpadesCommand>> undoCommands, redoCommands;
    public:

        void clear();
        bool canUndo() const;
        void undo(State& state);
        void addCommand(std::unique_ptr<SpadesCommand> command);
        //void addPlaceCommand(const Card& card);
        //void addBidCommand(int bid);
        //void addBidOptionCommand(const BidOption& bidOption, const Seat& seat);

    };
}
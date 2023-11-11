#pragma once
#include "history/SpadesCommand.h"
#include "history/SpadesCommandOrder.h"
#include <memory>
#include <vector>
#include <string>

namespace spd
{
    class SpadesHistory
    {
    public:
        SpadesCommandContainer undoCommandContainer;
        SpadesCommandContainer redoCommandContainer;
        void clear();
        bool canUndo() const;
        bool canRedo() const;
        void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController);
        void redo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController);
        void addAndExecuteBidCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, unsigned int bid);
        void addAndExecutePlaceCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, const Card& card);
        void addAndExecuteBidOptionCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, const Seat& seat, const BidOption& bidOption);

    };
}
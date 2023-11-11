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
        void undoBid(State &state, const TrumpVariationController &trumpVariationController);
        void undoCard(State &state, const TrumpVariationController &trumpVariationController);
        void redoBid(State &state, const TrumpVariationController &trumpVariationController);
        void redoCard(State &state, const TrumpVariationController &trumpVariationController);
    public:
        SpadesCommandContainer undoCommandContainer;
        SpadesCommandContainer redoCommandContainer;
        void clear();
        bool canUndo() const;
        bool canRedo() const;
        void undo(State &state, const TrumpVariationController &trumpVariationController);
        void redo(State &state, const TrumpVariationController &trumpVariationController);
        void addAndExecuteCommand(const SpadesCommandValue& commandValue, State &state, const TrumpVariationController &trumpVariationController);
    };
}
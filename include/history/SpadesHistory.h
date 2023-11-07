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
        void undo(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController);
        void redo(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController);
        void addCommand(std::unique_ptr<SpadesCommand> command);

    };
}
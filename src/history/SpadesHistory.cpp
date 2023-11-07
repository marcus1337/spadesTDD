#include "history/SpadesHistory.h"

using namespace spd;

void SpadesHistory::clear()
{
    undoCommands.clear();
    redoCommands.clear();
}

bool SpadesHistory::canUndo() const
{
    return !undoCommands.empty();
}
void SpadesHistory::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    std::unique_ptr<SpadesCommand> command = std::move(undoCommands.back());
    undoCommands.pop_back();
    command->undo(state, turn, trumpVariationController);
    redoCommands.push_back(std::move(command));
}
void SpadesHistory::redo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
}

void SpadesHistory::addCommand(std::unique_ptr<SpadesCommand> command)
{
    this->undoCommands.push_back(std::move(command));
}

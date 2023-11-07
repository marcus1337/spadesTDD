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
    std::unique_ptr<SpadesCommand> command = std::move(redoCommands.back());
    redoCommands.pop_back();
    command->execute(state, turn, trumpVariationController);
    undoCommands.push_back(std::move(command));
}

bool SpadesHistory::canRedo() const
{
    return !redoCommands.empty();
}

void SpadesHistory::addCommand(std::unique_ptr<SpadesCommand> command)
{
    undoCommands.push_back(std::move(command));
    redoCommands.clear();
}

bool SpadesHistory::deserialize(const std::string &encodedData)
{
}
std::string SpadesHistory::serialize() const
{
    return "";
}

#include "spades/history/SpadesHistory.h"
#include <sstream>

using namespace spd;

void SpadesHistory::clear()
{
}

void SpadesHistory::apply(State &state, const Move &move) const
{
}
void SpadesHistory::revert(State &state, const Move &move) const
{
}

void SpadesHistory::undo(State &state)
{
}
void SpadesHistory::redo(State &state)
{
}
bool SpadesHistory::canUndo() const
{
    return historyState.getLeftSize() > 0;
}
bool SpadesHistory::canRedo() const
{
    return historyState.getRightSize() > 0;
}
void SpadesHistory::addAndApply(State &state, const Move &move)
{
}

std::string SpadesHistory::serialize() const
{
    return historyState.serialize();
}
bool SpadesHistory::deserialize(const std::string &encoding) { return historyState.deserialize(encoding); }
#include "spades/history/SpadesHistory.h"
#include <sstream>

using namespace spd;

void SpadesHistory::clear()
{
    undoCommandContainer.clear();
    redoCommandContainer.clear();
}

void SpadesHistory::undoBid(State &state, const TrumpVariationController &trumpVariationController)
{
    auto commandValue = undoCommandContainer.bidValueVariants.back();
    undoCommandContainer.bidValueVariants.pop_back();
    SpadesCommandValueVisitor::undo(commandValue, state, trumpVariationController);
    redoCommandContainer.addCommandValue(commandValue);
}
void SpadesHistory::undoCard(State &state, const TrumpVariationController &trumpVariationController)
{
    auto commandValue = undoCommandContainer.placeCommandValues.back();
    undoCommandContainer.placeCommandValues.pop_back();
    SpadesCommandValueVisitor::undo(commandValue, state, trumpVariationController);
    redoCommandContainer.addCommandValue(commandValue);
}
void SpadesHistory::redoBid(State &state, const TrumpVariationController &trumpVariationController)
{
    auto commandValue = redoCommandContainer.bidValueVariants.back();
    redoCommandContainer.bidValueVariants.pop_back();
    SpadesCommandValueVisitor::execute(commandValue, state, trumpVariationController);
    undoCommandContainer.addCommandValue(commandValue);
}
void SpadesHistory::redoCard(State &state, const TrumpVariationController &trumpVariationController)
{
    auto commandValue = redoCommandContainer.placeCommandValues.back();
    redoCommandContainer.placeCommandValues.pop_back();
    SpadesCommandValueVisitor::execute(commandValue, state, trumpVariationController);
    undoCommandContainer.addCommandValue(commandValue);
}

bool SpadesHistory::canUndo() const
{
    return !undoCommandContainer.bidValueVariants.empty();
}

void SpadesHistory::undo(State &state, const TrumpVariationController &trumpVariationController)
{
    if (state.getRoundBids().empty() || !state.getPlayedRoundCards().empty())
    {
        undoCard(state, trumpVariationController);
    }
    else
    {
        undoBid(state, trumpVariationController);
    }
}

void SpadesHistory::redo(State &state, const TrumpVariationController &trumpVariationController)
{
    if (state.isBidPhase())
    {
        redoBid(state, trumpVariationController);
    }
    else
    {
        redoCard(state, trumpVariationController);
    }
}

bool SpadesHistory::canRedo() const
{
    return !redoCommandContainer.bidValueVariants.empty() || !redoCommandContainer.placeCommandValues.empty();
}

void SpadesHistory::addAndExecuteCommand(const SpadesCommandValue &commandValue, State &state, const TrumpVariationController &trumpVariationController)
{
    redoCommandContainer.clear();
    SpadesCommandValueVisitor::execute(commandValue, state, trumpVariationController);
    undoCommandContainer.addCommandValue(commandValue);
}

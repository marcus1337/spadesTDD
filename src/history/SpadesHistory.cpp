#include "history/SpadesHistory.h"
#include <sstream>

using namespace spd;

void SpadesHistory::clear()
{
    undoCommandContainer.clear();
    redoCommandContainer.clear();
}

bool SpadesHistory::canUndo() const
{
    return !undoCommandContainer.bidValueVariants.empty();
}
void SpadesHistory::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{

    if (state.getRoundBids().empty() || !state.getPlayedRoundCards().empty())
    {
        auto commandValue = undoCommandContainer.placeCommandValues.back();
        undoCommandContainer.placeCommandValues.pop_back();
        SpadesCommandValueVisitor::undo(commandValue, state, turn, trumpVariationController);
        redoCommandContainer.placeCommandValues.push_back(commandValue);
    }
    else
    {
        auto commandValue = undoCommandContainer.bidValueVariants.back();
        undoCommandContainer.bidValueVariants.pop_back();
        SpadesCommandValueVisitor::undo(commandValue, state, turn, trumpVariationController);
        redoCommandContainer.bidValueVariants.push_back(commandValue);
    }
}

void SpadesHistory::redo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    const auto roundBids = state.getRoundBids();
    const auto roundCards = state.getPlayedRoundCards();
    if (state.isBidPhase())
    {
        auto commandValue = redoCommandContainer.bidValueVariants.back();
        redoCommandContainer.bidValueVariants.pop_back();
        SpadesCommandValueVisitor::execute(commandValue, state, turn, trumpVariationController);
        undoCommandContainer.bidValueVariants.push_back(commandValue);
    }
    else
    {
        auto commandValue = redoCommandContainer.placeCommandValues.back();
        redoCommandContainer.placeCommandValues.pop_back();
        SpadesCommandValueVisitor::execute(commandValue, state, turn, trumpVariationController);
        undoCommandContainer.placeCommandValues.push_back(commandValue);
    }
}

bool SpadesHistory::canRedo() const
{
    return !redoCommandContainer.bidValueVariants.empty() || !redoCommandContainer.placeCommandValues.empty();
}

void SpadesHistory::addAndExecuteBidCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, unsigned int bid)
{
    redoCommandContainer.clear();
    BidCommandValue commandValue{bid};
    SpadesCommandValueVisitor::execute(commandValue, state, turn, trumpVariationController);
    undoCommandContainer.bidValueVariants.push_back(commandValue);
}
void SpadesHistory::addAndExecutePlaceCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, const Card &card)
{
    redoCommandContainer.clear();
    PlaceCommandValue commandValue{card};
    SpadesCommandValueVisitor::execute(commandValue, state, turn, trumpVariationController);
    undoCommandContainer.placeCommandValues.push_back(commandValue);
}
void SpadesHistory::addAndExecuteBidOptionCommand(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController, const Seat &seat, const BidOption &bidOption)
{
    redoCommandContainer.clear();
    BidOptionCommandValue commandValue{bidOption, seat};
    SpadesCommandValueVisitor::execute(commandValue, state, turn, trumpVariationController);
    undoCommandContainer.bidValueVariants.push_back(commandValue);
}
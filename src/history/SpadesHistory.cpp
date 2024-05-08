#include "spades/history/SpadesHistory.h"
#include <sstream>

using namespace spd;

void SpadesHistory::clear()
{
    historyState.clear();
}

void SpadesHistory::apply(State &state, const Move &move) const
{
    if (move.card.has_value())
    {
        state.playCard(move.seat, move.card.value());
    }
    else if (move.bid.has_value())
    {
        state.addBid(move.bid.value());
    }
    else if (move.bidOption.has_value())
    {
        state.setBidOption(move.seat, move.bidOption.value());
    }
}
void SpadesHistory::revert(State &state, const Move &move) const
{
    if (move.card.has_value())
    {
        state.popCard();
    }
    else if (move.bid.has_value())
    {
        state.popBid();
    }
    else if (move.bidOption.has_value())
    {
        state.removeBidOption(move.seat, move.bidOption.value());
    }
}

void SpadesHistory::undo(State &state)
{
    revert(state, historyState.peek());
    historyState.shiftLeft();
}

void SpadesHistory::redo(State &state)
{
    historyState.shiftRight();
    apply(state, historyState.peek());
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
    historyState.add(move);
    apply(state, move);
}

std::string SpadesHistory::serialize() const
{
    return historyState.serialize();
}
bool SpadesHistory::deserialize(const std::string &encoding) { return historyState.deserialize(encoding); }
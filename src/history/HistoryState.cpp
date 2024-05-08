#include "spades/history/HistoryState.h"

using namespace spd;

void HistoryState::clear()
{
    leftMoves = {};
    rightMoves = {};
}
void HistoryState::add(const Move &item)
{
    leftMoves.push(item);
    rightMoves = {};
}
const Move &HistoryState::peek() const
{
    return leftMoves.top();
}
int HistoryState::getLeftSize() const
{
    return leftMoves.size();
}
int HistoryState::getRightSize() const
{
    return rightMoves.size();
}
void HistoryState::shiftRight()
{
    leftMoves.push(rightMoves.top());
    rightMoves.pop();
}
void HistoryState::shiftLeft()
{
    rightMoves.push(leftMoves.top());
    leftMoves.pop();
}

std::string HistoryState::serialize() const
{
    return "";
}
bool HistoryState::deserialize(const std::string &encoding) { return false; }
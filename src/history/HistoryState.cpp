#include "spades/history/HistoryState.h"

using namespace spd;

void HistoryState::clear()
{
}
void HistoryState::add(const Move &item)
{
}
const Move &HistoryState::peek() const
{
    static Move mv{Seat::SOUTH, 1};
    return mv;
}
int HistoryState::getLeftSize() const
{
    return 0;
}
int HistoryState::getRightSize() const
{
    return 0;
}
void HistoryState::shiftRight()
{
}
void HistoryState::shiftLeft()
{
}

std::string HistoryState::serialize() const
{
    return "";
}
bool HistoryState::deserialize(const std::string &encoding) { return false; }
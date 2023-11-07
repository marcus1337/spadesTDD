#include "history/SpadesHistory.h"

using namespace spd;

void SpadesHistory::clear()
{
}

bool SpadesHistory::canUndo() const
{
    return false;
}
void SpadesHistory::undo(State& state)
{
}
/*void SpadesHistory::addPlaceCommand(const Card &card)
{
}
void SpadesHistory::addBidCommand(int bid)
{
}
void SpadesHistory::addBidOptionCommand(const BidOption &bidOption, const Seat &seat)
{
}*/
void SpadesHistory::addCommand(std::unique_ptr<SpadesCommand> command){
    this->undoCommands.push_back(std::move(command));
}


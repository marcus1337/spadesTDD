#include "history/SpadesCommandOrder.h"

using namespace spd;

std::vector<unsigned int> SpadesUndoCommands::getBidsData() const
{
    return {};
}
std::vector<unsigned int> SpadesUndoCommands::getCardsData() const
{
    return {};
}
std::vector<unsigned int> SpadesUndoCommands::getBidOptionsData() const
{
    return {};
}
void SpadesUndoCommands::setBidOptions(const std::vector<unsigned int> &data)
{
}
void SpadesUndoCommands::setBids(const std::vector<unsigned int> &data)
{
}
void SpadesUndoCommands::setPlacedCards(const std::vector<unsigned int> &data)
{
    
}

bool SpadesUndoCommands::hasValidCommandOrdering() const
{
    return false;
}
bool SpadesUndoCommands::canUndo() const
{
    return false;
}
void SpadesUndoCommands::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
}

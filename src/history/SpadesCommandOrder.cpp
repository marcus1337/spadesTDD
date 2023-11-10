#include "history/SpadesCommandOrder.h"

using namespace spd;

std::vector<unsigned int> SpadesUndoCommands::getBidsData() const
{
    std::vector<unsigned int> data;
    for (const auto &command : bidCommands)
    {
        data.push_back(command->serialize());
    }
    return data;
}

std::vector<unsigned int> SpadesUndoCommands::getCardsData() const
{
    std::vector<unsigned int> data;
    for (const auto &command : placeCommands)
    {
        data.push_back(command->serialize());
    }
    return data;
}

std::vector<unsigned int> SpadesUndoCommands::getBidOptionsData() const
{
    std::vector<unsigned int> data;
    for (const auto &[insertTime, command] : bidOptionCommandInsertions)
    {
        data.push_back(insertTime.round);
        data.push_back((unsigned int)insertTime.turnSeat);
        data.push_back(command->serialize());
    }
    return data;
}

void SpadesUndoCommands::setBidOptions(const std::vector<unsigned int> &data)
{
    bidOptionCommandInsertions.clear();
    for (int i = 0; i + 2 < data.size(); i += 2)
    {
        unsigned int roundData = data[i];
        unsigned int seatDataClamped = std::clamp<unsigned int>(data[i + 1], 0, 3);
        unsigned int bidOptionData = data[i + 2];
        InsertTime insertTime{.turnSeat = (Seat)(seatDataClamped), .round = roundData};
        auto command = std::make_unique<BidOptionCommand>(bidOptionData);
        bidOptionCommandInsertions.push_back(std::make_pair(insertTime, std::move(command)));
    }
}

void SpadesUndoCommands::setBids(const std::vector<unsigned int> &data)
{
    bidCommands.clear();
    for (const auto &value : data)
    {
        bidCommands.push_back(std::make_unique<BidCommand>(value));
    }
}

void SpadesUndoCommands::setPlacedCards(const std::vector<unsigned int> &data)
{
    placeCommands.clear();
    for (const auto &value : data)
    {
        placeCommands.push_back(std::make_unique<PlaceCommand>(value));
    }
}

bool SpadesUndoCommands::hasValidCommandOrdering() const
{
    return false;
}

bool SpadesUndoCommands::canUndoBid() const
{
    return false;
}

bool SpadesUndoCommands::canUndoDealtCard() const
{
    return false;
}

bool SpadesUndoCommands::canUndoBidOptionInsertion() const
{
    return false;
}

std::unique_ptr<BidCommand> SpadesUndoCommands::popBidCommand()
{
    auto removedPtr = std::move(bidCommands.back());
    bidCommands.pop_back();
    return std::move(removedPtr);
}

std::unique_ptr<PlaceCommand> SpadesUndoCommands::popPlaceCommand()
{
    auto removedPtr = std::move(placeCommands.back());
    bidOptionCommandInsertions.pop_back();
    return std::move(removedPtr);
}

std::pair<InsertTime, std::unique_ptr<BidOptionCommand>> SpadesUndoCommands::popBidOptionCommandInsertion()
{
    auto removedPtr = std::move(bidOptionCommandInsertions.back().second);
    InsertTime insertTime = bidOptionCommandInsertions.back().first;
    bidOptionCommandInsertions.pop_back();
    return std::make_pair(insertTime, std::move(removedPtr));
}

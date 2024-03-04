#include "spades/data/memento/SpadesMemento.h"
#include "spades/data/memento/json.h"
#include "spades/pch.h"

using namespace spd;

SpadesMemento::SpadesMemento(const SpadesCommandContainer &undoContainer, const SpadesCommandContainer &redoContainer, const TrumpVariationType &trumpVarType, const BidVariationType &bidVarType, unsigned int seed)
{
    this->seed = seed;
    this->bidVariationType = (unsigned int)bidVarType;
    this->trumpVariationType = (unsigned int)trumpVarType;
    this->undoBidsData = undoContainer.getBidsData();
    this->undoCardsData = undoContainer.getCardsData();
    this->redoBidsData = redoContainer.getBidsData();
    this->redoCardsData = redoContainer.getCardsData();
}
SpadesMemento::SpadesMemento(const std::string &data)
{
    deserialize(data);
}

BidVariationType SpadesMemento::getBidVariationType() const
{
    if (bidVariationType < (unsigned int)BidVariationType::LAST)
        return (BidVariationType)bidVariationType;
    else
        return BidVariationType{};
}

TrumpVariationType SpadesMemento::getTrumpVariationType() const
{
    if (trumpVariationType < (unsigned int)TrumpVariationType::LAST)
        return (TrumpVariationType)trumpVariationType;
    else
        return TrumpVariationType{};
}

std::string SpadesMemento::serialize() const
{
    nlohmann::json j;
    j[BID_VAR_KEY] = bidVariationType;
    j[TRUMP_VAR_KEY] = trumpVariationType;
    j[SEED_KEY] = seed;
    for (const auto &bid : undoBidsData)
    {
        j[UNDO_BIDS_KEY].push_back(bid);
    }
    for (const auto &card : undoCardsData)
    {
        j[UNDO_CARDS_KEY].push_back(card);
    }
    for (const auto &bid : redoBidsData)
    {
        j[REDO_BIDS_KEY].push_back(bid);
    }
    for (const auto &card : redoCardsData)
    {
        j[REDO_CARDS_KEY].push_back(card);
    }
    return j.dump();
}

void SpadesMemento::deserialize(const std::string &data)
{
    nlohmann::json j = nlohmann::json::parse(data);
    bidVariationType = j[BID_VAR_KEY];
    trumpVariationType = j[TRUMP_VAR_KEY];
    seed = j[SEED_KEY];
    deserializeArray(data, undoCardsData, UNDO_CARDS_KEY);
    deserializeArray(data, undoBidsData, UNDO_BIDS_KEY);
    deserializeArray(data, redoCardsData, REDO_CARDS_KEY);
    deserializeArray(data, redoBidsData, REDO_BIDS_KEY);
}

void SpadesMemento::deserializeArray(const std::string &data, std::vector<unsigned int> &arr, const char *KEY)
{
    nlohmann::json j = nlohmann::json::parse(data);
    arr.clear();
    if (j.find(KEY) != j.end() && j[KEY].is_array())
    {
        for (const auto &element : j[KEY])
        {
            arr.push_back(element);
        }
    }
}

unsigned int SpadesMemento::getSeed() const
{
    return seed;
}

SpadesCommandContainer SpadesMemento::getUndoContainer() const
{
    SpadesCommandContainer container;
    container.setBidValueVariants(undoBidsData);
    container.setPlacedCards(undoCardsData);
    return container;
}

SpadesCommandContainer SpadesMemento::getRedoContainer() const
{
    SpadesCommandContainer container;
    container.setBidValueVariants(redoBidsData);
    container.setPlacedCards(redoCardsData);
    return container;
}

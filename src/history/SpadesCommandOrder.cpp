#include "history/SpadesCommandOrder.h"

using namespace spd;

std::vector<unsigned int> SpadesCommandContainer::getBidsData() const
{
    std::vector<unsigned int> data;
    for (const auto &value : bidValueVariants)
    {
        data.push_back(SpadesCommandValueVisitor::serialize(value));
    }
    return data;
}

std::vector<unsigned int> SpadesCommandContainer::getCardsData() const
{
    std::vector<unsigned int> data;
    for (const auto &value : placeCommandValues)
    {
        data.push_back(SpadesCommandValueVisitor::serialize(value));
    }
    return data;
}

void SpadesCommandContainer::setBidValueVariants(const std::vector<unsigned int> &data)
{
    bidValueVariants.clear();
    for (const auto &value : data)
    {
        bidValueVariants.push_back(SpadesCommandValueVisitor::deserializeBidCommandValueVariant(value));
    }
}

void SpadesCommandContainer::setPlacedCards(const std::vector<unsigned int> &data)
{
    placeCommandValues.clear();
    for (const auto &value : data)
    {
        placeCommandValues.push_back(SpadesCommandValueVisitor::deserializePlaceCommandValue(value));
    }
}

void SpadesCommandContainer::clear()
{
    placeCommandValues.clear();
    bidValueVariants.clear();
}

void SpadesCommandContainer::addCommandValue(const SpadesCommandValue &commandValue)
{
    if (const auto placeCommandValue = std::get_if<PlaceCommandValue>(&commandValue))
    {
        placeCommandValues.push_back(*placeCommandValue);
    }
    else if (const auto bidCommandValueVariant = std::get_if<BidCommandValueVariant>(&commandValue))
    {
        bidValueVariants.push_back(*bidCommandValueVariant);
    }
}

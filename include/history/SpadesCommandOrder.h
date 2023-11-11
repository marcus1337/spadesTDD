#pragma once
#include "history/SpadesCommand.h"

namespace spd
{

    class SpadesCommandContainer
    {
    public:
        std::vector<BidCommandValueVariant> bidValueVariants;
        std::vector<PlaceCommandValue> placeCommandValues;
        std::vector<unsigned int> getBidsData() const;
        std::vector<unsigned int> getCardsData() const;
        void setBidValueVariants(const std::vector<unsigned int>& data);
        void setPlacedCards(const std::vector<unsigned int>& data);
        void clear();
        void addCommandValue(const SpadesCommandValue& commandValue);
    };

}
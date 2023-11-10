#pragma once
#include "history/SpadesCommand.h"

namespace spd
{
    struct InsertTime
    {
        Seat turnSeat;
        unsigned int round;
    };

    class SpadesUndoCommands
    {
        std::vector<std::unique_ptr<BidCommand>> bidCommands;
        std::vector<std::unique_ptr<PlaceCommand>> placeCommands;
        std::vector<std::pair<InsertTime, std::unique_ptr<BidOptionCommand>>> bidOptionCommandInsertions;

    public:
        std::vector<unsigned int> getBidsData() const;
        std::vector<unsigned int> getCardsData() const;
        std::vector<unsigned int> getBidOptionsData() const;
        void setBidOptions(const std::vector<unsigned int>& data);
        void setBids(const std::vector<unsigned int>& data);
        void setPlacedCards(const std::vector<unsigned int>& data);
        
        bool hasValidCommandOrdering() const;
        bool canUndoBid() const;
        bool canUndoDealtCard() const;
        bool canUndoBidOptionInsertion() const;
        std::unique_ptr<BidCommand> popBidCommand();
        std::unique_ptr<PlaceCommand> popPlaceCommand();
        std::pair<InsertTime, std::unique_ptr<BidOptionCommand>> popBidOptionCommandInsertion();
    
    };

}
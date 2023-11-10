#pragma once
#include "history/SpadesCommand.h"

namespace spd
{
    struct InsertTime
    {
        Seat turnSeat;
        int round;
    };

    class SpadesUndoCommands
    {
        std::vector<std::unique_ptr<BidCommand>> bidCommands;
        std::vector<std::unique_ptr<PlaceCommand>> placeCommands;
        std::vector<std::pair<InsertTime, std::unique_ptr<BidOptionCommand>>> bidOptionCommands;

    public:
        std::vector<unsigned int> getBidsData() const;
        std::vector<unsigned int> getCardsData() const;
        std::vector<unsigned int> getBidOptionsData() const;
        void setBidOptions(const std::vector<unsigned int>& data);
        void setBids(const std::vector<unsigned int>& data);
        void setPlacedCards(const std::vector<unsigned int>& data);
        
        bool hasValidCommandOrdering() const;
        bool canUndo() const;
        void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController);
    };

}
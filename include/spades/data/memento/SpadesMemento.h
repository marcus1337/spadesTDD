#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>

#include "spades/rules/BidVariationType.h"
#include "spades/rules/TrumpVariationType.h"
#include "spades/data/Seat.h"
#include "spades/data/card/Card.h"
#include "spades/history/SpadesHistory.h"

namespace spd
{
    class SpadesMemento
    {
        static constexpr const char *BID_VAR_KEY = "BidVariationType";
        static constexpr const char *TRUMP_VAR_KEY = "TrumpVariationType";
        static constexpr const char *SEED_KEY = "seed";
        static constexpr const char *UNDO_BIDS_KEY = "undo_bids";
        static constexpr const char *UNDO_CARDS_KEY = "undo_cards";
        static constexpr const char *REDO_BIDS_KEY = "redo_bids";
        static constexpr const char *REDO_CARDS_KEY = "redo_cards";

        unsigned int bidVariationType;
        unsigned int trumpVariationType;
        unsigned int seed;

        std::vector<unsigned int> undoBidsData;
        std::vector<unsigned int> undoCardsData;
        std::vector<unsigned int> redoBidsData;
        std::vector<unsigned int> redoCardsData;

        void deserializeArray(const std::string& data, std::vector<unsigned int>& arr, const char* KEY);

    public:
        SpadesMemento(const SpadesCommandContainer &undoContainer, const SpadesCommandContainer &redoContainer, const TrumpVariationType &trumpVarType, const BidVariationType &bidVarType, unsigned int seed);
        SpadesMemento(const std::string &data);
        std::string serialize() const;
        void deserialize(const std::string &data);

        unsigned int getSeed() const;
        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        SpadesCommandContainer getUndoContainer() const;
        SpadesCommandContainer getRedoContainer() const;
    };
}
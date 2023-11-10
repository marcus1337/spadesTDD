#pragma once

#include <vector>
#include "rules/BidVariationType.h"
#include "rules/TrumpVariationType.h"
#include <string>
#include "data/Seat.h"
#include "data/card/Card.h"
#include <set>
#include <map>
#include "data/State.h"
#include "history/SpadesHistory.h"

namespace spd
{
    class SpadesMemento
    {
        static constexpr const char *BID_VAR_KEY = "BidVariationType";
        static constexpr const char *TRUMP_VAR_KEY = "TrumpVariationType";
        static constexpr const char *SEED_KEY = "seed";
        static constexpr const char *BIDS_KEY = "bids";
        static constexpr const char *HISTORY_KEY = "history";

    public:

        SpadesMemento(const SpadesHistory& history, const State& state, const TrumpVariationType& trumpVarType, const BidVariationType& bidVarType, unsigned int seed);
        unsigned int bidVariationType;
        unsigned int trumpVariationType;
        unsigned int seed;
        std::vector<int> bids;
        std::vector<unsigned int> undoCommandsData;
        std::vector<unsigned int> redoCommandsData;
        std::vector<unsigned int> playedSeatCardPairData;
        std::vector<unsigned int> roundBidOptionsData;
        std::vector<unsigned int> trickTakersData;

        void setPlayedSeatCardPairsData(const std::vector<std::pair<Seat, Card>> &playedSeatCardPairs);
        void setRoundBidOptionsData(const std::map<int, std::set<std::pair<Seat, BidOption>>> &roundBidOptions);
        void setTrickTakersData(const std::vector<Seat> &seats);
        std::vector<std::pair<Seat, Card>> getPlayedSeatCardPairs() const;
        std::map<int, std::set<std::pair<Seat, BidOption>>> getRoundBidOptions() const;
        std::vector<Seat> getTrickTakers() const;

        SpadesMemento(const std::string &data);
        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
    };
}
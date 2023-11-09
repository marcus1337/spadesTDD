#pragma once

#include <vector>
#include "rules/BidVariationType.h"
#include "rules/TrumpVariationType.h"
#include <string>
#include "data/Seat.h"
#include "data/card/Card.h"
#include <set>
#include <map>

namespace spd
{
    class SpadesMemento
    {
        public:
        unsigned int bidVariationType;
        unsigned int trumpVariationType;
        unsigned int seed;
        std::vector<int> bids;
        std::string historyEncoding;

        std::vector<unsigned int> playedSeatCardPairData;
        std::vector<unsigned int> roundBidOptionsData;
        std::vector<unsigned int> trickTakersData;

        std::vector<std::pair<Seat, Card>> getPlayedSeatCardPairs() const;
        std::map<int, std::set<std::pair<Seat, BidOption>>> getRoundBidOptions() const;
        std::vector<Seat> getTrickTakers() const;

        SpadesMemento();
        SpadesMemento(const std::string &data);
        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
    };
}
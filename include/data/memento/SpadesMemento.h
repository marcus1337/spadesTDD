#pragma once

#include <vector>
#include "rules/BidVariationType.h"
#include "rules/TrumpVariationType.h"
#include <string>

namespace spd
{
    class SpadesMemento
    {
        public:
        unsigned int bidVariationType;
        unsigned int trumpVariationType;
        unsigned int seed;
        std::vector<int> bids;

        SpadesMemento();
        SpadesMemento(const std::string &data);
        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
    };
}
#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <limits>
#include "json.h"
#include "rules/BidVariation.h"
#include "rules/TrumpVariation.h"

namespace spd
{
    class SpadesMemento
    {
        static constexpr int UNSET_VALUE = std::numeric_limits<int>::min();
        static constexpr const char* bidVariationTypeKey = "BidVariationType";
        static constexpr const char* trumpVariationTypeKey = "TrumpVariationType";
        public:
        unsigned int bidVariationType;
        unsigned int trumpVariationType;
        unsigned int seed;
        std::vector<int> bids;

        SpadesMemento() : bidVariationType(UNSET_VALUE), trumpVariationType(UNSET_VALUE), seed(UNSET_VALUE)
        {
        }
        SpadesMemento(const std::string &data)
        {
            deserialize(data);
        }

        BidVariationType getBidVariationType() const
        {
            if (bidVariationType < (unsigned int)BidVariationType::LAST)
                return (BidVariationType)bidVariationType;
            else
                return BidVariationType{};
        }

        TrumpVariationType getTrumpVariationType() const
        {
            if (trumpVariationType < (unsigned int)TrumpVariationType::LAST)
                return (TrumpVariationType)trumpVariationType;
            else
                return TrumpVariationType{};
        }

        std::string serialize() const
        {
            assert(bidVariationType != UNSET_VALUE);
            assert(trumpVariationType != UNSET_VALUE);
            assert(seed != UNSET_VALUE);

            nlohmann::json j;
            j[bidVariationTypeKey] = bidVariationType;
            j[trumpVariationTypeKey] = trumpVariationType;
            j["seed"] = seed;
            for (const auto &bid : bids)
            {
                j["bids"].push_back(bid);
            }
            return j.dump();
        }

        void deserialize(const std::string &data)
        {
            nlohmann::json j = nlohmann::json::parse(data);
            bidVariationType = j[bidVariationTypeKey];
            trumpVariationType = j[trumpVariationTypeKey];
            seed = j["seed"];
            bids.clear();
            if (j.find("bids") != j.end() && j["bids"].is_array())
            {
                for (const auto &element : j["bids"])
                {
                    bids.push_back(element);
                }
            }
        }
    };
}
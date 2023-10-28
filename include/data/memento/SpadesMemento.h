#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator> 
#include <string>
#include <limits>
#include "json.h"

namespace spd
{
    struct SpadesMemento{
        static constexpr int UNSET_VALUE = std::numeric_limits<int>::min();
        unsigned int bidVariation;
        unsigned int trumpVariation;
        unsigned int seed;
        std::vector<int> bids;

        SpadesMemento() : bidVariation(UNSET_VALUE), trumpVariation(UNSET_VALUE), seed(UNSET_VALUE) {
        }
        SpadesMemento(const std::string& data) {
            deserialize(data);
        }

        std::string serialize() const{
            assert(bidVariation != UNSET_VALUE);
            assert(trumpVariation != UNSET_VALUE);
            assert(seed != UNSET_VALUE);

            nlohmann::json j;
            j["bidVariation"] = bidVariation;
            j["trumpVariation"] = trumpVariation;
            j["seed"] = seed;
            for (const auto &bid : bids) {
                j["bids"].push_back(bid);
            }
            return j.dump();
        }

        void deserialize(const std::string& data){
            nlohmann::json j = nlohmann::json::parse(data);
            bidVariation = j["bidVariation"];
            trumpVariation = j["trumpVariation"];
            seed = j["seed"];
            bids.clear();
            if (j.find("bids") != j.end() && j["bids"].is_array()) {
                for (const auto &element : j["bids"]) {
                    bids.push_back(element);
                }
            }
        }

    };
}
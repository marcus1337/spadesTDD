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
        unsigned int round;
        unsigned int bidVariation;
        unsigned int trumpVariation;
        unsigned int seed;

        SpadesMemento() : round(UNSET_VALUE), bidVariation(UNSET_VALUE), trumpVariation(UNSET_VALUE), seed(UNSET_VALUE) {
        }
        SpadesMemento(const std::string& data) {
            deserialize(data);
        }

        std::string serialize() const{
            assert(round != UNSET_VALUE);
            assert(bidVariation != UNSET_VALUE);
            assert(trumpVariation != UNSET_VALUE);
            assert(seed != UNSET_VALUE);

            nlohmann::json j;
            j["round"] = round;
            j["bidVariation"] = bidVariation;
            j["trumpVariation"] = trumpVariation;
            j["seed"] = seed;
            return j.dump();
        }

        void deserialize(const std::string& data){
            nlohmann::json j = nlohmann::json::parse(data);
            round = j["round"];
            bidVariation = j["bidVariation"];
            trumpVariation = j["trumpVariation"];
            seed = j["seed"];
        }

    };
}
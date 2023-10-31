#pragma once
#include "rules/TrumpVariation.h"
#include <map>
#include <memory>

namespace spd{
    enum class TrumpVariationType{
         ACE_HIGH = 0, LAST //TODO: JJ_ACE, JJ_DEUCE_ACE, JJ_DEUCE_DEUCE
    };
    class TrumpVariationController{
        TrumpVariationType variationType;
        std::map<TrumpVariationType, std::unique_ptr<TrumpVariation>> trumpVariations;
        const TrumpVariation* getTrumpVariation() const{
            return trumpVariations.at(variationType).get();
        }
        public:
        TrumpVariationController() : variationType((TrumpVariationType)0){
            trumpVariations[TrumpVariationType::ACE_HIGH] = std::make_unique<AceHigh>();
            assert(trumpVariations.size() == (int)TrumpVariationType::LAST);
        }
        TrumpVariationType getTrumpVariationType() const{
            return variationType;
        }      
        void setTrumpVariationType(TrumpVariationType type){
            this->variationType = type;
        }

        Seat getTrickTaker(const State& state) const {
            return getTrumpVariation()->getTrickTaker(state);
        }  
    };
}
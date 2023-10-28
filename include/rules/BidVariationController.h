#pragma once
#include "rules/BidVariation.h"

namespace spd{
    enum class BidVariationType{
        DOUBLE_BLIND_NILL = 0, DOUBLE_NILL, LAST
    };
    class BidVariationController{
        BidVariationType variationType;
        std::map<BidVariationType, std::unique_ptr<BidVariation>> bidVariations;
        const BidVariation* getBidVariation() const{
            return bidVariations.at(variationType).get();
        }
        public:
        BidVariationController() : variationType((BidVariationType)0){
            bidVariations[BidVariationType::DOUBLE_BLIND_NILL] = std::make_unique<DoubleBlindNil>();
            bidVariations[BidVariationType::DOUBLE_NILL] = std::make_unique<DoubleNil>();
            assert(bidVariations.size() == (int)BidVariationType::LAST);
        }
        std::vector<int> getPossibleBids(const Seat& seat, const State& state) const{
            return getBidVariation()->getPossibleBids(seat, state);
        }
        void setBidVariationType(BidVariationType type){
            this->variationType = type;
        }
        BidVariationType getBidVariationType() const{
            return variationType;
        }
    };
}
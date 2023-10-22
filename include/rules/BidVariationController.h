#pragma once
#include "rules/BidVariation.h"

namespace spd{
    enum class BidVariationType{
        DOUBLE_BLIND_NILL = 0, DOUBLE_NILL, LAST
    };
    class BidVariationController{
        BidVariationType variationType;
        std::map<BidVariationType, std::unique_ptr<BidVariation>> bidVariations;
        public:
        BidVariationController() : variationType((BidVariationType)0){
            bidVariations[BidVariationType::DOUBLE_BLIND_NILL] = std::make_unique<DoubleBlindNil>();
            bidVariations[BidVariationType::DOUBLE_NILL] = std::make_unique<DoubleNil>();
            assert(bidVariations.size() == (int)BidVariationType::LAST);
        }
        void setType(BidVariationType type){
            this->variationType = type;
        }
        void setType(int typeValue){
            if(typeValue >= 0 && typeValue < (int) BidVariationType::LAST){
                this->variationType = (BidVariationType)typeValue;
            }           
        }
        BidVariationType getBidVariationType() const{
            return variationType;
        }
        const BidVariation* getBidVariation() const{
            return bidVariations.at(variationType).get();
        }
        public:
        std::vector<Bid> getPossibleBids(const Player& player, const Player& teamPlayer, const Team& playerTeam, const Team& enemyTeam) const{
            return getBidVariation()->getPossibleBids(player, teamPlayer, playerTeam, enemyTeam);
        }
    };
}
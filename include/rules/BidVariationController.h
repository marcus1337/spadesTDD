#pragma once
#include "rules/BidVariation.h"
#include "rules/BidVariationType.h"
#include <memory>

namespace spd
{
    class BidVariationController
    {
        BidVariationType variationType;
        std::map<BidVariationType, std::unique_ptr<BidVariation>> bidVariations;
        const BidVariation *getBidVariation() const;

    public:
        BidVariationController();
        std::vector<int> getBids(const Seat &seat, const State &state) const;
        std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const;
        std::optional<int> getBidResult(const Seat &seat, const State &state) const;
        void setBidVariationType(BidVariationType type);
        BidVariationType getBidVariationType() const;
    };
}
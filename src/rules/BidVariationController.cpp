#include "rules/BidVariationController.h"

using namespace spd;

const BidVariation* BidVariationController::getBidVariation() const
{
    return bidVariations.at(variationType).get();
}

BidVariationController::BidVariationController() : variationType((BidVariationType)0)
{
    bidVariations[BidVariationType::DOUBLE_BLIND_NILL] = std::make_unique<DoubleBlindNil>();
    bidVariations[BidVariationType::DOUBLE_NILL] = std::make_unique<DoubleNil>();
    bidVariations[BidVariationType::MIRROR] = std::make_unique<Mirror>();
    bidVariations[BidVariationType::SUICIDE] = std::make_unique<Suicide>();
    assert(bidVariations.size() == (int)BidVariationType::LAST);
}
std::vector<int> BidVariationController::getBids(const Seat &seat, const State &state, const std::vector<Card>& hand) const
{
    return getBidVariation()->getBids(seat, state, hand);
}

std::vector<BidOption> BidVariationController::getBidOptions(const Seat &seat, const State &state) const
{
    return getBidVariation()->getBidOptions(seat, state);
}

std::optional<int> BidVariationController::getBidResult(const Seat &seat, const State &state) const
{
    return getBidVariation()->getBidResult(seat, state);
}

void BidVariationController::setBidVariationType(BidVariationType type)
{
    this->variationType = type;
}
BidVariationType BidVariationController::getBidVariationType() const
{
    return variationType;
}

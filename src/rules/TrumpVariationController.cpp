#include "rules/TrumpVariationController.h"
#include "rules/Trick.h"

using namespace spd;

const TrumpVariation *TrumpVariationController::getTrumpVariation() const
{
    return trumpVariations.at(variationType).get();
}
TrumpVariationController::TrumpVariationController() : variationType((TrumpVariationType)0)
{
    trumpVariations[TrumpVariationType::ACE_HIGH] = std::make_unique<AceHigh>();
    assert(trumpVariations.size() == (int)TrumpVariationType::LAST);
}
TrumpVariationType TrumpVariationController::getTrumpVariationType() const
{
    return variationType;
}
void TrumpVariationController::setTrumpVariationType(TrumpVariationType type)
{
    this->variationType = type;
}

Seat TrumpVariationController::getTrickTaker(const State &state) const
{
    return Trick(*getTrumpVariation(), state).getTrickTaker();
}

bool TrumpVariationController::canPlaceCard(const State &state, const Card &card, const Seat &seat) const
{
    return Trick(*getTrumpVariation(), state).canPlace(card, seat);
}
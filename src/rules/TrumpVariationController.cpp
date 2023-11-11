#include "rules/TrumpVariationController.h"
#include "rules/Trick.h"

using namespace spd;

const TrumpVariation *TrumpVariationController::getTrumpVariation() const
{
    return trumpVariations.at(variationType).get();
}
TrumpVariationController::TrumpVariationController() : variationType((TrumpVariationType)0)
{
    using enum TrumpVariationType;
    trumpVariations[ACE_HIGH] = std::make_unique<AceHigh>();
    trumpVariations[JOKER_JOKER] = std::make_unique<JokerJoker>();
    trumpVariations[JOKER_JOKER_DEUCE] = std::make_unique<JokerJokerDeuce>();
    trumpVariations[JOKER_JOKER_DEUCE_DEUCE] = std::make_unique<JokerJokerDeuceDeuce>();
    assert(trumpVariations.size() == (int)LAST);
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

bool TrumpVariationController::canPlaceCard(const State &state, const Card &card) const
{
    const auto turnSeat = state.getTurn(getTrickStartSeat(state));
    return Trick(*getTrumpVariation(), state).canPlace(turnSeat, card);
}

std::vector<Card> TrumpVariationController::getTrumpCardsOrderedByValueDescending() const
{
    return getTrumpVariation()->getTrumpCardsOrderedByValueDescending();
}

std::array<Card, 2> TrumpVariationController::getExcludedCards() const
{
    return getTrumpVariation()->getExcludedCards();
}

std::vector<Seat> TrumpVariationController::getTrickTakers(const State& state) const{
    std::vector<Seat> trickTakers;
    for(const auto trick : state.getTricks()){
        const auto trickTaker = Trick(*getTrumpVariation(), state).getTrickTaker();
        trickTakers.push_back(trickTaker);
    }
    return trickTakers;
}

Seat TrumpVariationController::getTrickStartSeat(const State& state) const{
    Seat seat = (Seat)0;
    const auto trickTakers = getTrickTakers(state);
    if(!trickTakers.empty()){
        seat = trickTakers.back();
    }
    return seat;
}

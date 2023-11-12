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

bool TrumpVariationController::canPlaceCard(const State &state, const Card &card, const std::vector<Card> &hand) const
{
    return Trick(*getTrumpVariation(), state).canPlace(card, hand);
}

std::vector<Card> TrumpVariationController::getTrumpCardsOrderedByValueDescending() const
{
    return getTrumpVariation()->getTrumpCardsOrderedByValueDescending();
}

std::array<Card, 2> TrumpVariationController::getExcludedCards() const
{
    return getTrumpVariation()->getExcludedCards();
}

std::vector<Seat> TrumpVariationController::getTrickTakers(const State &state) const
{
    std::vector<Seat> trickTakers;
    for (const auto &trick : state.getTricks())
    {
        const auto trickTaker = Trick(*getTrumpVariation(), state).getTrickTaker(trick);
        trickTakers.push_back(trickTaker);
    }
    return trickTakers;
}

Seat TrumpVariationController::getTrickStartSeat(const State &state) const
{
    Seat seat = (Seat)(state.getRound() % SeatUtils::numSeats);
    const auto trickTakers = getTrickTakers(state);
    const int tricksPerRound = 13;
    if (trickTakers.size() % tricksPerRound != 0)
    {
        seat = trickTakers.back();
    }
    return seat;
}

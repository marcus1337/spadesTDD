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
    const auto playedTrickCards = state.getCurrentTrickCardSeatPairs();
    const auto &trumpVariation = *getTrumpVariation();

    if (playedTrickCards.empty())
    {
        return (!trumpVariation.isTrumpCard(card) || trumpVariation.hasTrumpBeenPlayed(state.getPlayedCards(state.getRound()))) ? true : trumpVariation.areAllTrump(hand);
    }
    else
    {
        const auto leadCard = playedTrickCards.front().second;
        for (const auto &card : hand)
        {
            if (trumpVariation.isTrumpCard(card) && trumpVariation.isTrumpCard(leadCard))
            {
                return true;
            }
            const auto leadSuitOptional = leadCard.getSuit();
            const auto cardSuitOptional = card.getSuit();
            if (leadSuitOptional.has_value() && cardSuitOptional.has_value() && leadSuitOptional == cardSuitOptional)
            {
                return true;
            }
        }
        return false;
    }
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
        const auto seat = Trick(*getTrumpVariation(), state, trick).getTrickTaker();
        trickTakers.push_back(seat);
    }
    return trickTakers;
}

Seat TrumpVariationController::getTrickStartSeat(const State &state) const
{
    Seat startSeat = (Seat)(state.getRound() % SeatUtils::numSeats);
    const auto trickTakers = getTrickTakers(state);
    const int tricksPerRound = 13;
    if (trickTakers.size() % tricksPerRound != 0)
    {
        startSeat = trickTakers.back();
    }
    return startSeat;
}

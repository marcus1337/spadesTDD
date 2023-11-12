#include "rules/Trick.h"

using namespace spd;

bool TrickCardComparator::operator<(const TrickCardComparator &other) const
{
    if (trump != other.trump)
    {
        return other.trump;
    }
    else if (!trump && !other.trump && leadSuit != other.leadSuit)
    {
        return other.leadSuit;
    }
    else
    {
        return value < other.value;
    }
}

TrickCardComparator Trick::getTrickCardComparator(const Card &card) const
{
    return TrickCardComparator{.trump = trumpVariation.isTrumpCard(card), .leadSuit = isLeadSuit(card), .value = getValue(card)};
}

bool Trick::sameSuit(const Card &card1, const Card &card2) const
{
    const bool card1Trump = trumpVariation.isTrumpCard(card1);
    const bool card2Trump = trumpVariation.isTrumpCard(card2);
    if (card1Trump && card2Trump)
    {
        return true;
    }
    else if (!card1Trump && !card2Trump)
    {
        return card1.sameSuit(card2);
    }
    else
    {
        return false;
    }
}

Trick::Trick(const TrumpVariation &trumpVariation, const State &state) : trumpVariation(trumpVariation), state(state) {}

bool Trick::canPlaceFirst(const Card &card, const std::vector<Card> &hand) const
{
    return (!trumpVariation.isTrumpCard(card) || hasTrumpBeenPlayed()) ? true : hasOnlyTrumpCards(hand);
}

bool Trick::hasOnlyTrumpCards(const std::vector<Card> &hand) const
{
    bool hasOnlyTrumpCards = true;
    for (const auto &handCard : hand)
    {
        if (!trumpVariation.isTrumpCard(handCard))
        {
            hasOnlyTrumpCards = false;
        }
    }
    return hasOnlyTrumpCards;
}

bool Trick::hasTrumpBeenPlayed() const
{
    for (const auto &card : state.getPlayedCards(state.getRound()))
    {
        if (trumpVariation.isTrumpCard(card))
        {
            return true;
        }
    }
    return false;
}

bool Trick::canPlaceContinuation(const Card &card, const std::vector<Card> &hand) const
{
    const auto leadCard = getLeadCard().value();
    if (sameSuit(card, leadCard))
        return true;
    return !hasSameSuit(card, hand);
}

bool Trick::hasSameSuit(const Card &card, const std::vector<Card> &hand) const
{
    for (const auto &handCard : hand)
    {
        if (sameSuit(handCard, card))
            return true;
    }
    return false;
}

bool Trick::canPlace(const Card &card, const std::vector<Card> &hand) const
{
    const auto playedTrickCards = state.getCurrentTrickCardSeatPairs();
    return playedTrickCards.empty() ? canPlaceFirst(card, hand) : canPlaceContinuation(card, hand);
}

Seat Trick::getTrickTaker(const std::array<std::pair<Seat, Card>, SeatUtils::numSeats> &trick) const
{
    Card winCard = trick.front().second;
    Seat winSeat = trick.front().first;
    for (const auto &cardSeatPair : trick)
    {
        const Seat seat = cardSeatPair.first;
        const Card card = cardSeatPair.second;
        if (isNewTopCard(winCard, card))
        {
            winSeat = seat;
            winCard = card;
        }
    }

    return winSeat;
}

int Trick::getValue(const Card &card) const
{
    return trumpVariation.isTrumpCard(card) ? getTrumpValue(card) : getNormalValue(card);
}

int Trick::getNormalValue(const Card &card) const
{
    int value = 0;
    using enum Rank;
    const auto ranks = NormalCardValue::getRanks();
    for (int i = 1; i < ranks.size(); i++)
    {
        if (card.is(ranks[i]))
        {
            value += i - 1;
        }
    }
    if (card.is(Rank::ACE))
    {
        value += ranks.size();
    }
    return value;
}

int Trick::getTrumpValue(const Card &card) const
{
    int value = 0;
    const auto trumpCardsDescending = trumpVariation.getTrumpCardsOrderedByValueDescending();
    for (int i = 0; i < trumpCardsDescending.size(); i++)
    {
        if (card == trumpCardsDescending[i])
        {
            value += i;
        }
    }
    return value;
}

bool Trick::isNewTopCard(const Card &topCard, const Card &newCard) const
{
    const auto topCardComparator = getTrickCardComparator(topCard);
    const auto newCardComparator = getTrickCardComparator(newCard);
    return topCardComparator < newCardComparator;
}

std::optional<Card> Trick::getLeadCard() const
{
    const auto trickCardSeatPairs = state.getCurrentTrickCardSeatPairs();
    if (!trickCardSeatPairs.empty())
    {
        return std::make_optional(trickCardSeatPairs.front().second);
    }
    return std::nullopt;
}
std::optional<Suit> Trick::getLeadSuit() const
{
    const auto leadCardOptional = getLeadCard();
    if (leadCardOptional.has_value())
    {
        Card leadCard = leadCardOptional.value();
        if (trumpVariation.isTrumpCard(leadCard))
            return std::make_optional(Suit::SPADE);
        else
        {
            for (const auto suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART, Suit::SPADE})
                if (leadCard.is(suit))
                    return std::make_optional(suit);
        }
    }
    return std::nullopt;
}
bool Trick::isLeadSuit(const Card &card) const
{
    const auto leadCardOptional = getLeadCard();
    if (leadCardOptional.has_value())
    {
        const auto leadCard = leadCardOptional.value();
        return sameSuit(leadCard, card);
    }
    return false;
}
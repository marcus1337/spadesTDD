#include "rules/Trick.h"

using namespace spd;

bool TrickCardComparator::operator<(const TrickCardComparator &other) const
{
    if (trump != other.trump)
    {
        return !trump;
    }
    else if (!trump && leadSuit != other.leadSuit)
    {
        return leadSuit;
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
    const auto value1 = card1.getValue();
    if (trumpVariation.isTrumpCard(card1) && trumpVariation.isTrumpCard(card2))
    {
        return true;
    }
    else if (const auto normalCardValue = std::get_if<NormalCardValue>(&value1))
    {
        return card2.is(normalCardValue->getSuit());
    }
    return false;
}

Trick::Trick(const TrumpVariation &trumpVariation, const State &state) : trumpVariation(trumpVariation), state(state) {}

bool Trick::canPlaceFirst(const Card &card, const Seat &seat) const
{
    return (!trumpVariation.isTrumpCard(card) || hasTrumpBeenPlayed()) ? true : hasOnlyTrumpCards(seat);
}

bool Trick::hasOnlyTrumpCards(const Seat &seat) const
{
    bool hasOnlyTrumpCards = true;
    for (const auto &handCard : state.getHand(seat))
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

bool Trick::canPlaceContinuation(const Card &card, const Seat &seat) const
{
    const auto leadCard = getLeadCard().value();
    if (sameSuit(card, leadCard))
        return true;
    return !hasSameSuit(card, seat);
}

bool Trick::hasSameSuit(const Card &card, const Seat &seat) const
{
    for (const auto &handCard : state.getHand(seat))
    {
        if (sameSuit(handCard, card))
            return true;
    }
    return false;
}

bool Trick::canPlace(const Card &card, const Seat &seat) const
{
    const auto playedTrickCards = state.getPlayedTrickCards();
    return playedTrickCards.empty() ? canPlaceFirst(card, seat) : canPlaceContinuation(card, seat);
}

Seat Trick::getTrickTaker() const
{
    const auto trick = state.getTrick();
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
    int value = 0;
    if (trumpVariation.isTrumpCard(card))
    {
        value += trumpVariation.getTrumpValue(card);
    }

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

bool Trick::isNewTopCard(const Card &topCard, const Card &newCard) const
{
    const auto topCardComparator = getTrickCardComparator(topCard);
    const auto newCardComparator = getTrickCardComparator(newCard);
    return topCardComparator < newCardComparator;
}

std::optional<Card> Trick::getLeadCard() const
{
    const auto playedCards = state.getPlayedCardSeatPairs(state.getRound());
    if (!playedCards.empty())
    {
        return std::make_optional(playedCards.front().second);
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
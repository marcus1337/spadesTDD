#include "spades/rules/Trick.h"

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

Trick::Trick(const TrumpVariation &trumpVariation, const State &state, const std::array<std::pair<Seat, Card>, NUM_SEATS> &trick) : trumpVariation(trumpVariation), state(state), trick(trick) {}

bool Trick::hasSameSuit(const Card &card, const std::vector<Card> &hand) const
{
    for (const auto &handCard : hand)
    {
        if (sameSuit(handCard, card))
            return true;
    }
    return false;
}

Seat Trick::getTrickTaker() const
{
    Card winCard = trick.front().second;
    Seat winSeat = trick.front().first;
    for (const auto &cardSeatPair : trick)
    {
        const Seat& seat = cardSeatPair.first;
        const Card& card = cardSeatPair.second;
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
    return trumpVariation.isTrumpCard(card) ? trumpVariation.getTrumpValue(card) : trumpVariation.getRankValue(card);
}

bool Trick::isNewTopCard(const Card &topCard, const Card &newCard) const
{
    const auto& comp1 = getTrickCardComparator(topCard);
    const auto& comp2 = getTrickCardComparator(newCard);
    return comp1 < comp2;
}

std::optional<Suit> Trick::getLeadSuit() const
{
    const auto leadCard = trick.front().second;
    if (trumpVariation.isTrumpCard(leadCard))
        return Suit::SPADE;
    else
    {
        for (const auto suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART, Suit::SPADE})
            if (leadCard.is(suit))
                return suit;
    }
    return std::nullopt;
}
bool Trick::isLeadSuit(const Card &card) const
{
    return sameSuit(getLeadCard(), card);
}

Card Trick::getLeadCard() const
{
    return trick.front().second;
}
#include "rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

std::optional<Card> TrumpVariation::getLeadCard(const State &state) const
{
    for (const auto &cardSeatPair : state.getTrick())
    {
        if (!cardSeatPair.second.is(Joker::BIG) && !cardSeatPair.second.is(Joker::LITTLE))
        {
            return std::make_optional(cardSeatPair.second);
        }
    }
    return std::nullopt;
}
std::optional<Suit> TrumpVariation::getLeadSuit(const State &state) const
{
    const auto leadCard = getLeadCard(state);
    for (const auto suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART, Suit::SPADE})
        if (leadCard.has_value() && leadCard.value().is(suit))
            return std::make_optional(suit);
    return std::nullopt;
}

bool TrumpVariation::isLeadSuit(const Card &card, const State &state) const
{
    const auto leadSuit = getLeadSuit(state);
    if (leadSuit.has_value())
    {
        const Suit suit = leadSuit.value();
        return card.is(leadSuit.value());
    }
    return false;
}
bool TrumpVariation::isCardTrumpingWinCard(const Card &winCard, const Card &card, const State &state) const
{
    const bool winCardTrump = isTrumpCard(winCard);
    const bool cardTrump = isTrumpCard(card);
    const bool winCardLead = isLeadSuit(winCard, state);
    const bool cardLead = isLeadSuit(card, state);
    const int winCardValue = getCardValue(winCard);
    const int cardValue = getCardValue(card);
    return (!winCardTrump && cardTrump) ||
           (!winCardTrump && !cardTrump && !winCardLead && cardLead) ||
           (winCardTrump && cardTrump && winCardValue < cardValue) ||
           (!winCardTrump && !cardTrump && winCardValue < cardValue);
}

Seat TrumpVariation::getTrickTaker(const State &state) const
{
    const auto trick = state.getTrick();
    Card winCard = trick.front().second;
    Seat winSeat = trick.front().first;
    for (const auto &cardSeatPair : trick)
    {
        const Seat seat = cardSeatPair.first;
        const Card card = cardSeatPair.second;
        if (isCardTrumpingWinCard(winCard, card, state))
        {
            winSeat = seat;
            winCard = card;
        }
    }

    return winSeat;
}

AceHigh::AceHigh() = default;
bool AceHigh::isTrumpCard(const Card &card) const
{
    return card.is(Suit::SPADE);
}

int AceHigh::getCardValue(const Card &card) const
{
    using enum Rank;
    const auto ranks = NormalCardValue::getRanks();
    for (int i = 1; i < ranks.size(); i++)
    {
        if (card.is(ranks[i]))
        {
            return i - 1;
        }
    }
    if (card.is(Rank::ACE))
    {
        return ranks.size();
    }
    return 0;
}

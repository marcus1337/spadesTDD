#include "rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

int TrumpVariation::getCardValue(const Card &card) const
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

std::optional<Card> TrumpVariation::getLeadCard(const State &state) const
{
    const auto playedCards = state.getPlayedCards(state.getRound());
    if (!playedCards.empty())
    {
        return std::make_optional(playedCards.front().second);
    }
    return std::nullopt;
}
std::optional<Suit> TrumpVariation::getLeadSuit(const State &state) const
{
    const auto leadCardOptional = getLeadCard(state);
    if (leadCardOptional.has_value())
    {
        Card leadCard = leadCardOptional.value();
        if (isTrumpCard(leadCard))
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

bool TrumpVariation::isLeadSuit(const Card &card, const State &state) const
{
    const auto leadCardOptional = getLeadCard(state);
    if(leadCardOptional.has_value()){
        const auto leadCard = leadCardOptional.value();
        if(isTrumpCard(leadCard) && isTrumpCard(card)){
            return true;
        }else{
            const auto leadSuit = getLeadSuit(state).value_or(Suit::SPADE);
            return card.is(leadSuit);
        }
    }
    return false;
}
bool TrumpVariation::isNewTopCard(const Card &winCard, const Card &card, const State &state) const
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
        if (isNewTopCard(winCard, card, state))
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

int AceHigh::getTrumpCardValue(const Card &card) const
{
    return getCardValue(card);
}

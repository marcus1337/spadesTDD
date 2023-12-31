#include "spades/rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

bool TrumpVariation::isTrumpCard(const Card &card) const
{
    for (const auto &trumpCard : getTrumpCardsOrderedByValueDescending())
    {
        if (card == trumpCard)
        {
            return true;
        }
    }
    return false;
}

bool TrumpVariation::hasTrumpBeenPlayed(const std::vector<Card> &playedCards) const
{
    for (const auto &card : playedCards)
    {
        if (isTrumpCard(card))
        {
            return true;
        }
    }
    return false;
}

bool TrumpVariation::areAllTrump(const std::vector<Card> &cards) const
{
    bool onlyTrumps = true;
    for (const auto &handCard : cards)
    {
        if (!isTrumpCard(handCard))
        {
            onlyTrumps = false;
        }
    }
    return onlyTrumps;
}

bool TrumpVariation::sameSuit(const Card &cardA, const Card &cardB) const
{
    if (isTrumpCard(cardA) && isTrumpCard(cardB))
    {
        return true;
    }
    for (const auto &suit : {Suit::SPADE, Suit::DIAMOND, Suit::CLOVER, Suit::HEART})
    {
        if (cardA.is(suit) && cardB.is(suit))
        {
            return true;
        }
    }
    return false;
}

bool TrumpVariation::canPlaceCard(const std::vector<Card> &roundCards, const std::vector<Card> &trickCards, const std::vector<Card> &hand, const Card &card) const
{
    if (trickCards.empty())
    {
        return (!isTrumpCard(card) || hasTrumpBeenPlayed(roundCards)) ? true : areAllTrump(hand);
    }
    else
    {
        const auto leadCard = trickCards.front();
        bool hasLeadCardSuit = false;
        for (const auto &handCard : hand)
        {
            if (sameSuit(leadCard, handCard))
            {
                hasLeadCardSuit = true;
            }
        }
        return sameSuit(card, leadCard) || !hasLeadCardSuit;
    }
}

std::vector<Card> AceHigh::getTrumpCardsOrderedByValueDescending() const
{
    std::vector<Card> trumpCards;
    const auto ranks = NormalCardValue::getRanks();
    trumpCards.push_back(Card(ranks.front(), Suit::SPADE));
    for (int i = 12; i >= 1; i--)
    {
        trumpCards.push_back(Card(ranks[i], Suit::SPADE));
    }
    return trumpCards;
}

std::array<Card, 2> AceHigh::getExcludedCards() const
{
    return {Card(Joker::BIG), Card(Joker::LITTLE)};
}

std::vector<Card> JokerJoker::getTrumpCardsOrderedByValueDescending() const
{
    std::vector<Card> trumpCards;
    trumpCards.push_back(Card(Joker::BIG));
    trumpCards.push_back(Card(Joker::LITTLE));
    const auto ranks = NormalCardValue::getRanks();
    trumpCards.push_back(Card(ranks.front(), Suit::SPADE));
    for (int i = 12; i >= 1; i--)
    {
        trumpCards.push_back(Card(ranks[i], Suit::SPADE));
    }
    return trumpCards;
}
std::array<Card, 2> JokerJoker::getExcludedCards() const
{
    return {Card(Rank::TWO, Suit::CLOVER), Card(Rank::TWO, Suit::DIAMOND)};
}

std::vector<Card> JokerJokerDeuce::getTrumpCardsOrderedByValueDescending() const
{
    std::vector<Card> trumpCards;
    trumpCards.push_back(Card(Joker::BIG));
    trumpCards.push_back(Card(Joker::LITTLE));
    const auto ranks = NormalCardValue::getRanks();
    trumpCards.push_back(Card(ranks.at(1), Suit::SPADE));
    trumpCards.push_back(Card(ranks.front(), Suit::SPADE));
    for (int i = 12; i >= 2; i--)
    {
        trumpCards.push_back(Card(ranks[i], Suit::SPADE));
    }
    return trumpCards;
}
std::array<Card, 2> JokerJokerDeuce::getExcludedCards() const
{
    return {Card(Rank::TWO, Suit::CLOVER), Card(Rank::TWO, Suit::DIAMOND)};
}

std::vector<Card> JokerJokerDeuceDeuce::getTrumpCardsOrderedByValueDescending() const
{
    std::vector<Card> trumpCards;
    trumpCards.push_back(Card(Joker::BIG));
    trumpCards.push_back(Card(Joker::LITTLE));
    const auto ranks = NormalCardValue::getRanks();
    trumpCards.push_back(Card(ranks.at(1), Suit::DIAMOND));
    trumpCards.push_back(Card(ranks.at(1), Suit::SPADE));
    trumpCards.push_back(Card(ranks.front(), Suit::SPADE));
    for (int i = 12; i >= 2; i--)
    {
        trumpCards.push_back(Card(ranks[i], Suit::SPADE));
    }
    return trumpCards;
}
std::array<Card, 2> JokerJokerDeuceDeuce::getExcludedCards() const
{
    return {Card(Rank::TWO, Suit::CLOVER), Card(Rank::TWO, Suit::DIAMOND)};
}

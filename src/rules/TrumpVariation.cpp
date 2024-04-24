#include "spades/rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

void TrumpVariation::initCache()
{
    cachedTrumpCardsDescending = getTrumpCardsOrderedByValueDescending();
    for (const auto &card : cachedTrumpCardsDescending)
    {
        cachedTrumpCards.insert(card);
    }
}

const std::vector<Card> &TrumpVariation::getCachedTrumpCardsDescending() const
{
    return cachedTrumpCardsDescending;
}

int TrumpVariation::getTrumpValue(const Card &card) const
{
    auto it = std::find(cachedTrumpCardsDescending.begin(), cachedTrumpCardsDescending.end(), card);
    if (it != cachedTrumpCardsDescending.end())
    {
        return std::distance(cachedTrumpCardsDescending.begin(), it);
    }
    return 0;
}

int TrumpVariation::getRankValue(const Card &card) const
{
    if (!card.getRank().has_value())
    {
        return 0;
    }
    else
    {
        using enum Rank;
        Rank rank = card.getRank().value();
        switch (rank)
        {
        case Rank::TWO:
            return 1;
        case Rank::THREE:
            return 2;
        case Rank::FOUR:
            return 3;
        case Rank::FIVE:
            return 4;
        case Rank::SIX:
            return 5;
        case Rank::SEVEN:
            return 6;
        case Rank::EIGHT:
            return 7;
        case Rank::NINE:
            return 8;
        case Rank::TEN:
            return 9;
        case Rank::KNIGHT:
            return 10;
        case Rank::QUEEN:
            return 11;
        case Rank::KING:
            return 12;
        case Rank::ACE:
            return 13;
        default:
            return 0;
        }
    }
}

bool TrumpVariation::isTrumpCard(const Card &card) const
{
    return cachedTrumpCards.contains(card);
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

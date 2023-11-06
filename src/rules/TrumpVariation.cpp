#include "rules/TrumpVariation.h"
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

std::vector<Card> JokerJoker::getTrumpCardsOrderedByValueDescending() const{
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

std::vector<Card> JokerJokerDeuce::getTrumpCardsOrderedByValueDescending() const{
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

std::vector<Card> JokerJokerDeuceDeuce::getTrumpCardsOrderedByValueDescending() const{
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
#include "rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

AceHigh::AceHigh() = default;
bool TrumpVariation::isTrumpCard(const Card &card) const
{
    for(const auto& trumpCard : getTrumpCardsOrderedByValueDescending()){
        if(card == trumpCard){
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
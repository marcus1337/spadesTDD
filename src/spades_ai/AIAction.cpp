#include "spades_ai/AIAction.h"
#include <array>
#include <algorithm>

using namespace spd;

AIAction::AIAction(const Spades &spades) : spades(spades)
{
}

Card AIAction::getRandomCard()
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}

Card AIAction::getCard(std::vector<float> netOutput) const
{
    assert(netOutput.size() == 8);
    assert(!spades.getPlaceableCards().empty());
    std::array<std::pair<int, float>, 4> suitIndices;
    for (int i = 0; i < 4; i++)
    {
        suitIndices[i] = std::make_pair(i, netOutput[i]);
    }
    auto comparator = [](const std::pair<int, float> &a, const std::pair<int, float> &b)
    {
        return a.second > b.second;
    };
    std::sort(suitIndices.begin(), suitIndices.end(), comparator);

    for (const auto &suitIndex : suitIndices)
    {
        const auto placeableCards = getPlaceableCardsAscending((Suit)suitIndex.first);
        const auto cardStrength = netOutput.at(suitIndex.first + 4);
        if (!placeableCards.empty())
        {
            std::size_t cardIndex = (std::size_t)std::roundf(cardStrength * (float)placeableCards.size());
            cardIndex = std::clamp<std::size_t>(cardIndex, 0, placeableCards.size() - 1);
            return placeableCards[cardIndex];
        }
    }

    return Card();
}

std::vector<Card> AIAction::getPlaceableCards(const Suit &suit) const
{
    std::vector<Card> cards;
    if (suit == Suit::SPADE)
    {
        for (const auto &card : spades.getHand(spades.getTurnSeat()))
        {
            for (const auto &trumpCard : spades.getTrumpCardsDescending())
            {
                if (trumpCard == card)
                {
                    cards.push_back(trumpCard);
                }
            }
        }
    }
    else
    {
        for (const auto &card : spades.getHand(spades.getTurnSeat()))
        {
            if (card.is(suit))
            {
                cards.push_back(card);
            }
        }
    }
    return cards;
}

std::vector<Card> AIAction::getPlaceableCardsAscending(const Suit &suit) const
{
    auto cards = getPlaceableCards(suit);
    auto compare = [&](const Card &a, const Card &b)
    {
        int trumpValueA = getTrumpValue(a);
        int trumpValueB = getTrumpValue(b);
        if (trumpValueA == 0 && trumpValueB == 0)
            return getRankValue(a) < getRankValue(b);
        return trumpValueA < trumpValueB;
    };
    std::sort(cards.begin(), cards.end(), compare);
    return cards;
}

int AIAction::getTrumpValue(const Card &card) const
{
    const auto trumps = spades.getTrumpCardsDescending();
    for (int i = 0; i < trumps.size(); i++)
    {
        if (card == trumps[i])
        {
            return trumps.size() - i;
        }
    }
    return 0;
}
int AIAction::getRankValue(const Card &card) const
{
    int value = spades.getRanksDescending().size();
    for (const auto &rank : spades.getRanksDescending())
    {
        if (card.is(rank))
            return value;
        else
            value--;
    }
    return 0;
}
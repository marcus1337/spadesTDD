#include "spades/rules/Deck.h"
#include "spades/pch.h"

using namespace spd;

bool Deck::isExcluded(const Card &card) const
{
    for (const auto &excludedCard : excludeCards)
        if (excludedCard == card)
            return true;
    return false;
}

std::vector<Card> Deck::getCards() const
{
    std::vector<Card> cards;
    assert(Card::getCards().size() == DECK_SIZE + 2);
    for (const auto &card : Card::getCards())
        if (!isExcluded(card))
            cards.push_back(card);
    assert(cards.size() == DECK_SIZE);
    return cards;
}

std::vector<Card> Deck::getShuffledCards(int round) const
{
    portableRandom.setSeed(getSeed());
    std::vector<Card> cards = getCards();
    assert(cards.size() == DECK_SIZE);
    for (int i = 0; i <= round; i++)
        portableRandom.shuffle(cards);
    return cards;
}

Deck::Deck() : excludeCards({Card(Joker::LITTLE), Card(Joker::BIG)})
{
}

void Deck::setExcludeCards(std::array<Card, NUM_EXCLUDED_CARDS> excludeCards)
{
    assert(excludeCards[0] != excludeCards[1]);
    this->excludeCards = excludeCards;
}

void Deck::setSeed(unsigned int seed)
{
    portableRandom.setSeed(seed);
}

unsigned int Deck::getSeed() const
{
    return portableRandom.getSeed();
}

std::array<Card, HAND_SIZE> Deck::getHand(const Seat &seat, int round) const
{
    const auto cards = getShuffledCards(round);
    const int deckOffset = static_cast<int>(seat) * HAND_SIZE;
    std::array<Card, HAND_SIZE> hand{};
    for (int i = 0; i < hand.size(); i++)
    {
        hand[i] = cards.at(i + deckOffset);
    }
    return hand;
}

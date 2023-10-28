#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "PortableRandom.h"
#include <array>
#include "data/card/Card.h"
#include "data/Seat.h"

namespace spd
{
    class Deck
    {
        mutable PortableRandom portableRandom;
        std::array<Card, 2> excludeCards;

        bool isExcluded(const Card &card) const
        {
            for (const auto &excludedCard : excludeCards)
                if (excludedCard == card)
                    return true;
            return false;
        }

        std::vector<Card> getCards() const
        {
            std::vector<Card> cards;
            for (const auto &card : Card::getCards())
                if (!isExcluded(card))
                    cards.push_back(card);
            return cards;
        }

        std::vector<Card> getShuffledCards(int round) const
        {
            portableRandom.setSeed(getSeed());
            std::vector<Card> cards = getCards();
            for (int i = 0; i <= round; i++)
                portableRandom.shuffle(cards);
            return cards;
        }

    public:
        Deck() : excludeCards({Card(Joker::LITTLE), Card(Joker::BIG)})
        {
        }

        void setExcludeCards(std::array<Card, 2> excludeCards)
        {
            assert(excludeCards[0] != excludeCards[1]);
            this->excludeCards = excludeCards;
        }

        std::array<Card, 13> getHand(const Seat &seat, int round) const
        {
            const auto cards = getShuffledCards(round);
            const int handSize = 13;
            const int deckOffset = static_cast<int>(seat) * handSize;
            std::array<Card, 13> hand{};
            for(int i = 0; i < hand.size(); i++){
                hand[i] = cards.at(i + deckOffset);
            }
            return hand;
        }

        void setSeed(unsigned int seed)
        {
            portableRandom.setSeed(seed);
        }

        unsigned int getSeed() const
        {
            return portableRandom.getSeed();
        }

    };
}
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include "PortableRandom.h"
#include <array>
#include "data/Player.h"

namespace spd
{
    class Deck
    {
        PortableRandom portableRandom;
        std::array<Card, 2> excludeCards;

        std::vector<Suit> getSuits() const
        {
            return {spd::Suit::CLOVER, spd::Suit::DIAMOND, spd::Suit::HEART, spd::Suit::SPADE};
        }

        std::vector<Rank> getRanks() const
        {
            std::vector<Rank> ranks;
            for (int i = 0; i < 15; i++)
            {
                ranks.push_back((Rank)i);
            }
            return ranks;
        }

        bool vectorContainsCard(const std::vector<Card> &cards, const Card &card) const
        {
            return std::find(cards.begin(), cards.end(), card) != cards.end();
        }

        std::vector<Card> getDefaultCards() const
        {
            std::vector<Card> cards;
            for (const auto &suit : getSuits())
            {
                for (const auto &rank : getRanks())
                {
                    if (!Card::isJoker(rank))
                    {
                        cards.push_back(Card(rank, suit));
                    }
                }
            }
            return cards;
        }
        std::array<Card, 2> getJokers() const
        {
            return {Card::BigJoker(), Card::LittleJoker()};
        }

        void addCardsExcluding(std::vector<Card> cards, const std::vector<Card> cardsToAdd) const
        {
            for (const auto &card : cardsToAdd)
            {
                if (excludeCards[0] != card && excludeCards[1] != card)
                {
                    cards.push_back(card);
                }
            }
        }

        std::vector<Card> getCards() const
        {
            std::vector<Card> cards;
            addCardsExcluding(cards, getDefaultCards());
            const auto jokers = getJokers();
            addCardsExcluding(cards, {jokers[0], jokers[1]});
            return cards;
        }

        std::vector<Card> getShuffledCards(int round)
        {
            std::vector<Card> cards = getCards();
            setSeed(getSeed());
            for (int i = 0; i <= round; i++)
            {
                portableRandom.shuffle(cards);
            }
            return cards;
        }

    public:
        Deck() : excludeCards(getJokers())
        {
        }

        void setExcludeCards(std::array<Card, 2> excludeCards)
        {
            assert(excludeCards[0] != excludeCards[1]);
            this->excludeCards = excludeCards;
        }

        std::vector<Card> getHand(const Seat &seat, int round)
        {
            const auto cards = getShuffledCards(round);
            const int handSize = 13;
            const int deckOffset = static_cast<int>(seat) * handSize;
            std::vector<Card> hand;

            auto start = std::begin(cards) + deckOffset;
            auto end = std::begin(cards) + std::min(deckOffset + handSize, static_cast<int>(cards.size()));

            std::copy(start, end, std::back_inserter(hand));
            return hand;
        }

        void setSeed(int seed)
        {
            portableRandom.setSeed(seed);
        }
        int getSeed() const
        {
            return portableRandom.getSeed();
        }
    };
}
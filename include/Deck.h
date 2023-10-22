#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include "global/PortableRandom.h"

namespace spd
{
    class Deck {
        std::vector<Card> cards;
        std::vector<Suit> getSuits() const {
            using enum Suit;
            return { CLOVER, DIAMOND, HEART, SPADE };
        }
        std::vector<Rank> getRanks() const {
            using enum Rank;
            return { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, KNIGHT, QUEEN, KING, BIG_JOKER, LITTLE_JOKER };
        }

        bool vectorContainsCard(const std::vector<Card>& cards, const Card& card) const {
            return std::find(cards.begin(), cards.end(), card) != cards.end();
        }

        std::vector<Card> getDefaultCards() const {
            std::vector<Card> cards;
            for (const auto& suit : getSuits()) {
                for (const auto& rank : getRanks()) {
                    if (!Card::isJoker(rank)) {
                        cards.push_back(Card(rank, suit));
                    }
                }
            }
            return cards;
        }
        std::vector<Card> getJokers() const {
            return { Card::BigJoker(), Card::LittleJoker() };
        }

        void addCardsExcluding(const std::vector<Card>& addCards, const std::vector<Card>& excludeCards) {
            for (const auto& card : addCards) {
                if (!vectorContainsCard(excludeCards, card)) {
                    cards.push_back(card);
                }
            }
        }

        void setDeckExcluding(const std::vector<Card>& excludeCards) {
            addCardsExcluding(getDefaultCards(), excludeCards);
            addCardsExcluding(getJokers(), excludeCards);
        }

        void shuffle() {
            PortableRandom::getInstance().shuffle(cards);
        }

    public:
        Deck() {
            setDeckExcluding(getJokers());
            shuffle();
        }
        Deck(const std::vector<Card>& excludeCards) {
            setDeckExcluding(excludeCards);
            shuffle();
        }
        Card pop() {
            const Card card = cards.back();
            cards.pop_back();
            return card;
        }
        bool empty() const {
            return cards.empty();
        }

    };
}
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
    class Deck {
        PortableRandom portableRandom;
        std::array<Card, 2> excludeCards;
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
        std::array<Card, 2> getJokers() const {
            return { Card::BigJoker(), Card::LittleJoker() };
        }

        void addCardsExcluding(std::vector<Card> cards, const std::vector<Card> cardsToAdd) const{
            for (const auto& card : cardsToAdd) {
                if(excludeCards[0] != card && excludeCards[1] != card){
                    cards.push_back(card);
                }
            }
        }

        std::vector<Card> getCards() const {
            std::vector<Card> cards;
            addCardsExcluding(cards, getDefaultCards());
            const auto jokers = getJokers();
            addCardsExcluding(cards, {jokers[0], jokers[1]});
            return cards;
        }

        std::vector<Card> getShuffledCards(int round) {
            std::vector<Card> cards = getCards();
            setSeed(getSeed());
            for(int i = 0 ; i <= round; i++){
                portableRandom.shuffle(cards);
            }
            return cards;
        }

    public:
        Deck() : excludeCards(getJokers()) {
        }

        void setExcludeCards(std::array<Card, 2> excludeCards){
            assert(excludeCards[0] != excludeCards[1]);
            this->excludeCards = excludeCards;
        }
        
        std::array<Card, 13> getHand(const Seat& seat, int round){
            setSeed(getSeed());
            const int handSize = 13;
            std::array<Card, handSize> hand = {};
            const auto cards = getShuffledCards(round);
            const int deckOffset = ((int)seat) * handSize;
            std::copy(cards.begin() + deckOffset, cards.begin() + handSize + deckOffset, hand.begin());
            return hand;
        }

        void setSeed(int seed){
            portableRandom.setSeed(seed);
        }
        int getSeed() const {
            return portableRandom.getSeed();
        }
    };
}
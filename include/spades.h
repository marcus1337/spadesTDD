#pragma once
#include <vector>
#include <cassert>
#include <algorithm>

namespace spds
{

    enum class Rank {
        ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, KNIGHT, QUEEN, KING, BIG_JOKER, LITTLE_JOKER
    };
    enum class Suit {
        SPADE, HEART, DIAMOND, CLOVER, NONE
    };

    struct Card {
        const Rank rank;
        const Suit suit;
        Card(const Rank& rank, const Suit& suit) : rank(rank), suit(suit) {
            isJoker(rank) ? assert(suit == Suit::NONE) : assert(suit != Suit::NONE);
        }
        static Card BigJoker() {
            return Card(Rank::BIG_JOKER, Suit::NONE);
        }
        static Card LittleJoker() {
            return Card(Rank::LITTLE_JOKER, Suit::NONE);
        }
        static bool isJoker(const Rank& rank) {
            return rank == Rank::BIG_JOKER || rank == Rank::LITTLE_JOKER;
        }
        bool operator==(const Card& other) const {
            return (rank == other.rank) && (suit == other.suit);
        }
    };

    struct Deck {
        std::vector<Card> cards;
        std::vector<Suit> getSuits() const {
            using enum Suit;
            return { CLOVER, DIAMOND, HEART, SPADE };
        }
        std::vector<Rank> getRanks() const {
            using enum Rank;
            return { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, KNIGHT, QUEEN, KING, BIG_JOKER, LITTLE_JOKER };
        }

        bool vectorContainsCard(const std::vector<Card>& cards, const Card& card) {
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

        Deck() {
            setDeckExcluding(getJokers());
        }

    };

    struct CardEvaluator {
        virtual int getValue(const Card& card) = 0;
    };

    struct BidRule {
        virtual std::vector<int> getPossibleBids() = 0;
    };

    class Spades
    {

    public:
        int getTestNumber() const;
    };
}
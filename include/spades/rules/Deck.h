#pragma once
#include <vector>
#include <array>
#include "spades/data/PortableRandom.h"
#include "spades/data/card/Card.h"
#include "spades/data/Seat.h"

namespace spd
{
    class Deck
    {
    public:
        static constexpr unsigned int NUM_UNIQUE_CARDS = 54;
        static constexpr unsigned int DECK_SIZE = 52;
        static constexpr unsigned int HAND_SIZE = DECK_SIZE / SeatUtils::numSeats;
        static constexpr unsigned int NUM_EXCLUDED_CARDS = NUM_UNIQUE_CARDS - DECK_SIZE;

    private:
        mutable PortableRandom portableRandom;
        std::array<Card, NUM_EXCLUDED_CARDS> excludeCards;

        bool isExcluded(const Card &card) const;
        std::vector<Card> getCards() const;
        std::vector<Card> getShuffledCards(int round) const;

    public:
        Deck();
        void setExcludeCards(std::array<Card, NUM_EXCLUDED_CARDS> excludeCards);
        void setSeed(unsigned int seed);
        unsigned int getSeed() const;
        std::array<Card, HAND_SIZE> getHand(const Seat &seat, int round) const;
    };
}
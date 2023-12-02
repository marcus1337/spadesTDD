#pragma once
#include <vector>
#include <array>
#include "spades/data/PortableRandom.h"
#include "spades/data/card/Card.h"
#include "spades/data/Seat.h"
#include "spades/rules/Constants.h"

namespace spd
{
    class Deck
    {
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
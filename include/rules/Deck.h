#pragma once
#include <vector>
#include "table/PortableRandom.h"
#include <array>
#include "data/card/Card.h"
#include "data/Seat.h"

namespace spd
{
    class Deck
    {
        mutable PortableRandom portableRandom;
        std::array<Card, 2> excludeCards;

        bool isExcluded(const Card &card) const;
        std::vector<Card> getCards() const;
        std::vector<Card> getShuffledCards(int round) const;
    public:        
        Deck();
        void setExcludeCards(std::array<Card, 2> excludeCards);
        void setSeed(unsigned int seed);
        unsigned int getSeed() const;
        std::array<Card, 13> getHand(const Seat &seat, int round) const;
    };
}
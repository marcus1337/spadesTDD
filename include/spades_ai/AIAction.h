#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"
#include "spades_ai/Analyze.h"
#include "spades/data/PortableRandom.h"

namespace spd
{
    class AIAction
    {
        static PortableRandom portableRandom;
        const Spades &spades;

        int getTrumpValue(const Card& card) const;
        int getRankValue(const Card& card) const;
        std::vector<Card> getPlaceableCards(const Suit& suit) const;
        std::vector<Card> getPlaceableCardsAscending(const Suit& suit) const;

    public:
        AIAction(const Spades &spades);
        Card getRandomCard();
        Card getCard(std::vector<float> netOutput) const;
    };
}
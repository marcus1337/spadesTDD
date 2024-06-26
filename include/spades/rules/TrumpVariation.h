
#pragma once
#include <vector>
#include <array>
#include <optional>
#include "spades/data/card/Card.h"
#include "spades/data/Seat.h"
#include "spades/data/State.h"

namespace spd
{

    class TrumpVariation
    {
    private:
        std::vector<Card> cachedTrumpCardsDescending;
        std::set<Card> cachedTrumpCards;

    protected:
        bool sameSuit(const Card &cardA, const Card &cardB) const;
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const = 0;

    public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
        bool isTrumpCard(const Card &card) const;
        virtual std::array<Card, 2> getExcludedCards() const = 0;
        bool hasTrumpBeenPlayed(const std::vector<Card> &roundCards) const;
        bool areAllTrump(const std::vector<Card> &cards) const;
        bool canPlaceCard(const std::vector<Card> &roundCards, const std::vector<Card> &trickCards, const std::vector<Card> &hand, const Card &card) const;
        const std::vector<Card> &getCachedTrumpCardsDescending() const;
        void initCache();
        int getTrumpValue(const Card& card) const;
        int getRankValue(const Card& card) const;
    };

    class AceHigh : public TrumpVariation
    {
    public:
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const override;
        virtual std::array<Card, 2> getExcludedCards() const override;
    };

    class JokerJoker : public TrumpVariation
    {
    public:
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const override;
        virtual std::array<Card, 2> getExcludedCards() const override;
    };
    class JokerJokerDeuce : public TrumpVariation
    {
    public:
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const override;
        virtual std::array<Card, 2> getExcludedCards() const override;
    };
    class JokerJokerDeuceDeuce : public TrumpVariation
    {
    public:
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const override;
        virtual std::array<Card, 2> getExcludedCards() const override;
    };
}

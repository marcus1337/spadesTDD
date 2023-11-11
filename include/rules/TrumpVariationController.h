#pragma once
#include "rules/TrumpVariation.h"
#include <map>
#include <memory>
#include "rules/TrumpVariationType.h"

namespace spd{
    class TrumpVariationController{
        TrumpVariationType variationType;
        std::map<TrumpVariationType, std::unique_ptr<TrumpVariation>> trumpVariations;
        const TrumpVariation* getTrumpVariation() const;
        public:
        TrumpVariationController();
        TrumpVariationType getTrumpVariationType() const;
        void setTrumpVariationType(TrumpVariationType type);
        Seat getTrickTaker(const State& state) const;
        bool canPlaceCard(const State& state, const Card& card, const std::vector<Card>& hand) const;
        std::vector<Card> getTrumpCardsOrderedByValueDescending() const;
        std::array<Card, 2> getExcludedCards() const;
        std::vector<Seat> getTrickTakers(const State& state) const;
        Seat getTrickStartSeat(const State& state) const;
    };
}
#pragma once
#include "spades/rules/TrumpVariation.h"
#include <map>
#include <memory>
#include "spades/rules/TrumpVariationType.h"

namespace spd{
    class TrumpVariationController{
        TrumpVariationType variationType;
        std::map<TrumpVariationType, std::unique_ptr<TrumpVariation>> trumpVariations;
        mutable std::vector<Seat> trickTakersCache;

        const TrumpVariation* getTrumpVariation() const;
        public:
        TrumpVariationController();
        TrumpVariationType getTrumpVariationType() const;
        void setTrumpVariationType(TrumpVariationType type);
        bool canPlaceCard(const State& state, const Card& card, const std::vector<Card>& hand) const;
        const std::vector<Card>& getTrumpCardsOrderedByValueDescending() const;
        std::array<Card, 2> getExcludedCards() const;
        std::vector<Seat> getTrickTakers(const State& state) const;
        Seat getTrickStartSeat(const State& state) const;
    };
}
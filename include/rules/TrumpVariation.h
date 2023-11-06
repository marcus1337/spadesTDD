
#pragma once
#include <vector>
#include <array>
#include <optional>
#include "data/card/Card.h"
#include "data/Seat.h"
#include "data/State.h"

namespace spd
{

    class TrumpVariation
    {
    protected:
    public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
        bool isTrumpCard(const Card &card) const;
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const = 0;
        virtual std::array<Card, 2> getExcludedCards() const = 0;
    };

    class AceHigh : public TrumpVariation
    {

    public:
        AceHigh();
        virtual std::vector<Card> getTrumpCardsOrderedByValueDescending() const override;
        virtual std::array<Card, 2> getExcludedCards() const override;

    };
}

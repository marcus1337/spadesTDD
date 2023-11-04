
#pragma once
#include <vector>
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
        virtual bool isTrumpCard(const Card &card) const = 0;
        virtual int getTrumpValue(const Card &card) const = 0;
    };

    class AceHigh : public TrumpVariation
    {

    public:
        AceHigh();
        virtual bool isTrumpCard(const Card &card) const override;
        virtual int getTrumpValue(const Card &card) const override;
    };
}

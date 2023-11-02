
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
        std::optional<Card> getLeadCard(const State &state) const;
        std::optional<Suit> getLeadSuit(const State &state) const;
        virtual bool isTrumpCard(const Card &card) const = 0;
        virtual int getCardValue(const Card &card) const = 0;
        bool isLeadSuit(const Card &card, const State &state) const;
        bool isCardTrumpingWinCard(const Card &winCard, const Card &card, const State &state) const;

    public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
        virtual Seat getTrickTaker(const State &state) const;
    };

    class AceHigh : public TrumpVariation
    {

    public:
        AceHigh();
        virtual bool isTrumpCard(const Card &card) const override;
        virtual int getCardValue(const Card &card) const override;
    };
}

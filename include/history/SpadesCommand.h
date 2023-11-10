#pragma once
#include "data/State.h"
#include "rules/TrumpVariationController.h"
#include "rules/Turn.h"
#include <string>

namespace spd
{
    class SpadesCommand
    {
    public:
        SpadesCommand() = default;
        virtual ~SpadesCommand() = default;
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) = 0;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) = 0;
        virtual unsigned int serialize() const = 0;
    };

    class PlaceCommand : public SpadesCommand
    {
        const Card card;

    public:
        PlaceCommand(const Card &card);
        explicit PlaceCommand(int cardValue);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual unsigned int serialize() const override;
        Card getCard() const;
    };

    class BidCommand : public SpadesCommand
    {
        const int bid;

    public:
        explicit BidCommand(int bid);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual unsigned int serialize() const override;
        int getBid() const;
    };

    class BidOptionCommand : public SpadesCommand
    {
        const BidOption bidOption;
        const Seat seat;
        std::pair<BidOption,Seat> deserialize(int serializedValue) const;
    public:
        BidOptionCommand(const BidOption& bidOption, const Seat& seat);
        explicit BidOptionCommand(int serializedValue);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual unsigned int serialize() const override;
        BidOption getBidOption() const;
        Seat getSeat() const;
    };

}
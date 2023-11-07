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
        virtual std::vector<unsigned int> serialize() const = 0;
    };

    class PlaceCommand : public SpadesCommand
    {
        const Card card;

    public:
        PlaceCommand(const Card &card);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual std::vector<unsigned int> serialize() const override;
    };

    class BidCommand : public SpadesCommand
    {
        const int bid;

    public:
        BidCommand(int bid);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual std::vector<unsigned int> serialize() const override;
    };

    class BidOptionCommand : public SpadesCommand
    {
        const BidOption bidOption;
        const Seat seat;
    public:
        BidOptionCommand(const BidOption& bidOption, const Seat& seat);
        virtual void execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController) override;
        virtual std::vector<unsigned int> serialize() const override;
    };

}
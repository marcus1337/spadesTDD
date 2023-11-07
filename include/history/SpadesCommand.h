#pragma once
#include "data/State.h"
#include "rules/TrumpVariationController.h"
#include "rules/Turn.h"

namespace spd
{

    class SpadesCommand
    {
    public:
        virtual ~SpadesCommand() = default;
        virtual void execute(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController) = 0;
        virtual void undo(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController) = 0;
    };

    class PlaceCommand : public SpadesCommand{
        const Card card;
        public:
        PlaceCommand(const Card& card);
        virtual void execute(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController) override;
        virtual void undo(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController) override;
    };

}
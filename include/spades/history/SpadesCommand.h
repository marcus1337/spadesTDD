#pragma once
#include "spades/data/State.h"
#include "spades/rules/TrumpVariationController.h"
#include <string>
#include <variant>

namespace spd
{
    struct PlaceCommandValue
    {
        const Card card;
    };

    struct BidCommandValue
    {
        const unsigned int bid;
    };

    struct BidOptionCommandValue
    {
        const BidOption bidOption;
        const Seat seat;
    };

    using BidCommandValueVariant = std::variant<BidCommandValue, BidOptionCommandValue>;
    using SpadesCommandValue = std::variant<PlaceCommandValue, BidCommandValueVariant>;

    class SpadesCommandValueVisitor
    {
        static constexpr unsigned int BID_OPTION_ENCODING_OFFSET = 0xFF;
        static unsigned int serialize(const BidOptionCommandValue &value);
    public:
        static unsigned int serialize(const std::variant<PlaceCommandValue, BidCommandValueVariant> &value);
        static PlaceCommandValue deserializePlaceCommandValue(unsigned int data);
        static BidCommandValueVariant deserializeBidCommandValueVariant(unsigned int data);
        static void execute(const SpadesCommandValue& commandValue, State &state, const TrumpVariationController &trumpVariationController);
        static void undo(const BidCommandValueVariant &bidCommandValueVariant, State &state, const TrumpVariationController &trumpVariationController);
        static void undo(const PlaceCommandValue &placeCommandValue, State &state, const TrumpVariationController &trumpVariationController);
    };
}

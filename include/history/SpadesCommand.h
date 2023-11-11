#pragma once
#include "data/State.h"
#include "rules/TrumpVariationController.h"
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

    class SpadesCommandValueVisitor
    {
        static constexpr unsigned int BID_OPTION_ENCODING_OFFSET = 0xFF;

        static unsigned int serialize(const BidOptionCommandValue &value)
        {
            return static_cast<unsigned int>(value.bidOption) * static_cast<unsigned int>(SeatUtils::numSeats) + static_cast<unsigned int>(value.seat);
        }

    public:
        static unsigned int serialize(const std::variant<PlaceCommandValue, BidCommandValueVariant> &value)
        {
            unsigned int serializedValue = 0;
            if (const auto placeCommandValue = std::get_if<PlaceCommandValue>(&value))
            {
                serializedValue = placeCommandValue->card.serialize();
            }
            else if (const auto bidCommandValueVariant = std::get_if<BidCommandValueVariant>(&value))
            {
                if (const auto bidCommandValue = std::get_if<BidCommandValue>(bidCommandValueVariant))
                {
                    serializedValue = bidCommandValue->bid;
                }
                else if (const auto bidOptionCommandValue = std::get_if<BidOptionCommandValue>(bidCommandValueVariant))
                {
                    serializedValue = serialize(*bidOptionCommandValue) + BID_OPTION_ENCODING_OFFSET;
                }
            }
            return serializedValue;
        }

        static PlaceCommandValue deserializePlaceCommandValue(unsigned int data)
        {
            return PlaceCommandValue{.card = Card(data)};
        }

        static BidCommandValueVariant deserializeBidCommandValueVariant(unsigned int data)
        {
            if (data >= BID_OPTION_ENCODING_OFFSET)
            {
                data -= BID_OPTION_ENCODING_OFFSET;
                unsigned int seatValue = data % SeatUtils::numSeats;
                unsigned int bidOptValue = (data - seatValue) / SeatUtils::numSeats;
                if (bidOptValue >= (unsigned int)BidOption::LAST)
                {
                    bidOptValue = 0;
                }
                return BidOptionCommandValue{.bidOption = (BidOption)bidOptValue, .seat = (Seat)seatValue};
            }
            else
            {
                return BidCommandValue{.bid = data};
            }
        }

        static void execute(const BidCommandValueVariant &bidCommandValueVariant, State &state, const TrumpVariationController &trumpVariationController)
        {
            if (const auto bidCommandValue = std::get_if<BidCommandValue>(&bidCommandValueVariant))
            {
                state.addBid(bidCommandValue->bid);
            }
            else if (const auto bidOptionCommandValue = std::get_if<BidOptionCommandValue>(&bidCommandValueVariant))
            {
                state.setBidOption(bidOptionCommandValue->seat, bidOptionCommandValue->bidOption);
            }
        }
        static void execute(const PlaceCommandValue &placeCommandValue, State &state, const TrumpVariationController &trumpVariationController)
        {
            state.playCard(placeCommandValue.card);
            if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
            {
                state.trickTakers.push_back(trumpVariationController.getTrickTaker(state));
            }
        }

        static void undo(const BidCommandValueVariant &bidCommandValueVariant, State &state, const TrumpVariationController &trumpVariationController)
        {
            if (const auto bidCommandValue = std::get_if<BidCommandValue>(&bidCommandValueVariant))
            {
                state.popBid();
            }
            else if (const auto bidOptionCommandValue = std::get_if<BidOptionCommandValue>(&bidCommandValueVariant))
            {
                state.removeBidOption(bidOptionCommandValue->seat, bidOptionCommandValue->bidOption);
            }
        }
        static void undo(const PlaceCommandValue &placeCommandValue, State &state, const TrumpVariationController &trumpVariationController)
        {
            if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
            {
                state.trickTakers.pop_back();
            }
            state.playedSeatCardPairs.pop_back();
        }
    };

}
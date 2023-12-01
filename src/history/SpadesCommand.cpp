#include "spades/history/SpadesCommand.h"

using namespace spd;

unsigned int SpadesCommandValueVisitor::serialize(const BidOptionCommandValue &value)
{
    return static_cast<unsigned int>(value.bidOption) * static_cast<unsigned int>(SeatUtils::numSeats) + static_cast<unsigned int>(value.seat);
}

unsigned int SpadesCommandValueVisitor::serialize(const std::variant<PlaceCommandValue, BidCommandValueVariant> &value)
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

PlaceCommandValue SpadesCommandValueVisitor::deserializePlaceCommandValue(unsigned int data)
{
    return PlaceCommandValue{.card = Card(data)};
}

BidCommandValueVariant SpadesCommandValueVisitor::deserializeBidCommandValueVariant(unsigned int data)
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

void SpadesCommandValueVisitor::execute(const SpadesCommandValue &commandValue, State &state, const TrumpVariationController &trumpVariationController)
{
    if (const auto placeCommandValue = std::get_if<PlaceCommandValue>(&commandValue))
    {
        const auto trickStartSeat = trumpVariationController.getTrickStartSeat(state);
        state.playCard(state.getTrickTurn(trickStartSeat), placeCommandValue->card);
    }
    else if (const auto bidVariant = std::get_if<BidCommandValueVariant>(&commandValue))
    {
        if (const auto bidOptValue = std::get_if<BidOptionCommandValue>(bidVariant))
        {
            state.setBidOption(bidOptValue->seat, bidOptValue->bidOption);
        }
        else if (const auto bidCommandValue = std::get_if<BidCommandValue>(bidVariant))
        {
            state.addBid(bidCommandValue->bid);
        }
    }
}

void SpadesCommandValueVisitor::undo(const BidCommandValueVariant &bidCommandValueVariant, State &state, const TrumpVariationController &trumpVariationController)
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

void SpadesCommandValueVisitor::undo(const PlaceCommandValue &placeCommandValue, State &state, const TrumpVariationController &trumpVariationController)
{
    state.popCard();
}

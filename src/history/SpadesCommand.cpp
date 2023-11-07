#include "history/SpadesCommand.h"

using namespace spd;

PlaceCommand::PlaceCommand(const Card &card) : card(card) {}
void PlaceCommand::execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.playCard(turn.getTurnSeat(state), card);
    if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
    {
        state.trickTakers.push_back(trumpVariationController.getTrickTaker(state));
    }
}

void PlaceCommand::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
    {
        state.trickTakers.pop_back();
    }
    state.playedSeatCardPairs.pop_back();
}

BidCommand::BidCommand(int bid) : bid(bid)
{
}
void BidCommand::execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.bids.push_back(bid);
}
void BidCommand::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.bids.pop_back();
}

BidOptionCommand::BidOptionCommand(const BidOption& bidOption, const Seat& seat) : bidOption(bidOption), seat(seat)
{
}
void BidOptionCommand::execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.setBidOption(seat, bidOption);
}
void BidOptionCommand::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.roundBidOptions[state.getRound()].erase(std::make_pair(seat, bidOption));
}
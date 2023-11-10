#include "history/SpadesCommand.h"

using namespace spd;

PlaceCommand::PlaceCommand(const Card &card) : card(card) {}
PlaceCommand::PlaceCommand(int cardValue) : card(Card(cardValue)) {}
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

BidCommand::BidCommand(int bid) : bid(std::clamp<int>(bid, 0, 13))
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

int BidCommand::getBid() const
{
    return bid;
}

BidOptionCommand::BidOptionCommand(const BidOption &bidOption, const Seat &seat) : bidOption(bidOption), seat(seat)
{
}

BidOptionCommand::BidOptionCommand(int serializedValue) : seat(deserialize(serializedValue).second), bidOption(deserialize(serializedValue).first)
{
}

std::pair<BidOption, Seat> BidOptionCommand::deserialize(int serializedValue) const
{
    int seatValue = abs(serializedValue % SeatUtils::numSeats);
    int bidOptValue = abs((serializedValue - seatValue) / SeatUtils::numSeats);
    if (bidOptValue > (int)BidOption::SHOW_HAND)
    {
        bidOptValue = 0;
    }
    return std::make_pair((BidOption)bidOptValue, (Seat)seatValue);
}

void BidOptionCommand::execute(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.setBidOption(seat, bidOption);
}
void BidOptionCommand::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    state.roundBidOptions[state.getRound()].erase(std::make_pair(seat, bidOption));
}

int BidCommand::serialize() const
{
    return bid;
}

int PlaceCommand::serialize() const
{
    return card.serialize();
}

Card PlaceCommand::getCard() const
{
    return card;
}

int BidOptionCommand::serialize() const
{
    return ((int)bidOption) * SeatUtils::numSeats + (int)seat;
}

BidOption BidOptionCommand::getBidOption() const
{
    return bidOption;
}
Seat BidOptionCommand::getSeat() const
{
    return seat;
}

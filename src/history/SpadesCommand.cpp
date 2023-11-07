#include "history/SpadesCommand.h"

using namespace spd;

PlaceCommand::PlaceCommand(const Card &card) : card(card) {}
void PlaceCommand::execute(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController){
    state.playCard(turn.getTurnSeat(state), card);
    if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
    {
        state.trickTakers.push_back(trumpVariationController.getTrickTaker(state));
    }
}

void PlaceCommand::undo(State &state, const Turn& turn, const TrumpVariationController& trumpVariationController){
    if (state.getPlayedTrickCardSeatPairs().size() == SeatUtils::numSeats)
    {
        state.trickTakers.pop_back();
    }
    state.playedSeatCardPairs.pop_back();
}

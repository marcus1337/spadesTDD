#include "spades_ai/AIObservation.h"

using namespace spd;

bool AIObservation::isLastTrickCard() const
{
    return spades.getCurrentTrickCardSeatPairs().size() == NUM_SEATS - 1;
}

bool AIObservation::canPlaceTopCard(const Seat &seat) const
{
    for (const auto &card : spades.getHand(seat))
    {
        if (spades.isTopCardIfPlaced(card))
        {
            return true;
        }
    }
    return false;
}

bool AIObservation::canPlaceNonTopCard(const Seat &seat) const
{
    for (const auto &card : spades.getHand(seat))
    {
        if (!spades.isTopCardIfPlaced(card))
        {
            return true;
        }
    }
    return false;
}

bool AIObservation::isDefendingNil(const Seat &seat) const
{
    const int numTakenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    const bool nilBid = spades.getBidResult(seat).value();
    return nilBid && numTakenTricks == 0;
}

bool AIObservation::isNilBidAtRisk(const Seat &seat) const
{
    const auto topSeat = spades.getCurrentTrickTopSeat();
    return topSeat.has_value() && topSeat.value() == seat && isDefendingNil(seat);
}

AIObservation::AIObservation(const Spades &spades) : spades(spades)
{
}

bool AIObservation::canSabotageOpponentNil() const
{
    const auto seat = spades.getTurnSeat();
    if (isLastTrickCard())
    {
        for (const auto &opponent : {SeatUtils::getRightOpponentSeat(seat), SeatUtils::getLeftOpponentSeat(seat)})
        {
            if (isNilBidAtRisk(opponent))
            {
                return canPlaceNonTopCard(seat);
            }
        }
    }
    return false;
}

bool AIObservation::canDefendTeamNil() const
{
    const auto seat = spades.getTurnSeat();
    return !isDefendingNil(seat) &&
           isNilBidAtRisk(SeatUtils::getTeamSeat(seat)) &&
           canPlaceTopCard(seat);
}

bool AIObservation::teamNeedTricks() const
{
    const auto seat = spades.getTurnSeat();
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    const int countedTricks = spades.getCountedRoundTricks(seat) + spades.getCountedRoundTricks(teamSeat);
    const int teamBid = spades.getBidResult(seat).value() + spades.getBidResult(teamSeat).value();
    return teamBid > countedTricks;
}

bool AIObservation::opponentTeamNeedTricks() const
{
    const auto leftOpponent = SeatUtils::getLeftOpponentSeat(spades.getTurnSeat());
    const auto rightOpponent = SeatUtils::getRightOpponentSeat(spades.getTurnSeat());
    const int countedTricks = spades.getCountedRoundTricks(leftOpponent) + spades.getCountedRoundTricks(rightOpponent);
    const int opponentTeamBid = spades.getBidResult(leftOpponent).value() + spades.getBidResult(rightOpponent).value();
    return opponentTeamBid > countedTricks;
}

// bool in (4): spadesBroken, any opponent need tricks, need tricks, team player need tricks,
// bool in (3): any opponent protects nil, team player protects nil, protecting nil
// bool in (12): player might have suit (suits*num_other_players)
// bool in (4): has_topcard_in_hand(#suits)
// bool in (4): has_lowcard_in_hand(#suits)
// bool in (4): lead_suit(#suits)
// float in (4): percentage_of_remaining_cards_in_hand(#suits), example: out of all remaining cards of type #suit - how large perc. in my hand? (special case when no remaining cards: input is 0)
// float in (4): percentage_of_remaining_cards(#suits), example: out of all remaining cards how large percentage is of type #suit?
// out (8): should place suit{TRUMP,D,H,C}? Card strength?(0..1) - {TRUMP,D,H,C}
std::vector<float> AIObservation::getNetInput() const
{
    std::vector<float> input;
    input.push_back(spades.isSpadesBroken() ? 1.0f : 0.0f);

    return input;
}
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

// bool in  (1): spadesBroken,
// bool in  (2): any opponent need tricks, team need tricks,
// bool in  (3): any opponent protects nil, team player protects nil, protecting nil
// bool in  (12): player might have suit (suits*num_other_players)
// bool in  (4): current_lead_suit(#suits)
// bool in  (3): #num_placed_trick_cards
// float in (4): percentage_of_remaining_cards_in_hand(#suits), example: out of all remaining cards of type #suit - how large perc. in my hand? (special case when no remaining cards: input is 0)
std::vector<float> AIObservation::getNetInput() const
{
    std::vector<float> input;
    input.push_back(spades.isSpadesBroken() ? 1.f : 0);
    input.push_back(opponentTeamNeedTricks() ? 1.f : 0);
    input.push_back(teamNeedTricks() ? 1.f : 0);

    const auto seat = spades.getTurnSeat();
    const auto oppSeat1 = SeatUtils::getLeftOpponentSeat(seat);
    const auto oppSeat2 = SeatUtils::getRightOpponentSeat(seat);
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    const bool oppTeamNil = isDefendingNil(oppSeat1) || isDefendingNil(oppSeat1);
    input.push_back(isDefendingNil(seat) ? 1.f : 0);
    input.push_back(oppTeamNil ? 1.f : 0);
    input.push_back(isDefendingNil(teamSeat) ? 1.f : 0);

    for (const auto &s : {oppSeat1, oppSeat2, teamSeat})
    {
        for (const auto &suit : Card::getSuits())
        {
            input.push_back(hasSkippedLeadSuit(suit, s) ? 1.f : 0);
        }
    }

    std::array<bool, 4> leadSuitCheck = {false, false, false, false};
    const auto trickPairs = spades.getCurrentTrickCardSeatPairs();
    if (!trickPairs.empty())
    {
        const auto leadCard = trickPairs.front().second;
        if (spades.isTrumpCard(leadCard))
        {
            leadSuitCheck[(int)Suit::SPADE] = true;
        }
        else
        {
            leadSuitCheck[(int)leadCard.getSuit().value_or(Suit::SPADE)] = true;
        }
    }
    for (const auto &value : leadSuitCheck)
    {
        input.push_back(value);
    }
    for (int i = 1; i <= 3; i++)
    {
        input.push_back(trickPairs.size() == i ? true : false);
    }

    const auto unplacedCards = spades.getUnplacedRoundCards();
    for (const auto &suit : Card::getSuits())
    {
        const auto unplacedSuitCards = filterCards(suit, unplacedCards);
        const auto handSuitCards = filterCards(suit, spades.getHand(seat));
        const float value = unplacedCards.empty() ? 0 : (float)handSuitCards.size() / (float)unplacedSuitCards.size();
        input.push_back(value);
    }

    return input;
}

bool AIObservation::sameSuit(const Card &c1, const Card &c2) const
{
    if (!spades.isTrumpCard(c1) && !spades.isTrumpCard(c2))
    {
        return c1.getSuit().has_value() && c2.getSuit().has_value() && c1.getSuit().value() == c2.getSuit().value();
    }
    return spades.isTrumpCard(c1) && spades.isTrumpCard(c2);
}

bool AIObservation::hasSuit(const Suit &suit, const Card &card) const
{
    if (suit == Suit::SPADE)
    {
        return spades.isTrumpCard(card) || (card.getSuit().has_value() && card.getSuit().value() == suit);
    }
    else
    {
        return card.getSuit().has_value() && card.getSuit().value() == suit;
    }
}

std::vector<Card> AIObservation::filterCards(const Suit &suit, const std::vector<Card> &cards) const
{
    std::vector<Card> filtered;
    for (const auto &card : cards)
    {
        if (hasSuit(suit, card))
        {
            filtered.push_back(card);
        }
    }
    return filtered;
}

bool AIObservation::hasSkippedLeadSuit(const Suit &leadSuit, const Seat &seat) const
{ // TODO: Make it work for half-finished tricks
    const auto csPairs = spades.getCurrentRoundCardSeatPairs();
    for (int trickIndex = 0; trickIndex < csPairs.size() / 4; trickIndex++)
    {
        int from = trickIndex * 4;
        Card leadCard = csPairs[from].second;
        if (hasSuit(leadSuit, leadCard))
        {
            for (int i = 1; i <= 3; i++)
            {
                const auto &otherSeat = csPairs[from + i].first;
                const auto &otherCard = csPairs[from + i].second;
                if (otherSeat == seat && !hasSuit(leadSuit, otherCard))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

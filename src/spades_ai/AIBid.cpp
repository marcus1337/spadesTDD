#include "spades_ai/AIBid.h"

using namespace spd;

AIBid::AIBid(const Spades &spades) : spades(spades), analyze(spades)
{
}

int AIBid::getClosestNonZeroBid(int targetBid) const
{
    const auto possibleBids = spades.getPossibleBids();
    int bid = possibleBids.front();
    for (const auto possibleBid : possibleBids)
    {
        if (bid == 0 && possibleBid != 0)
        {
            bid = possibleBid;
        }
        else if (abs(possibleBid - targetBid) < abs(bid - targetBid))
        {
            bid = possibleBid;
        }
    }
    return bid;
}

bool AIBid::hasWeakHand() const
{
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        if (analyze.isStrongStartCard(card))
        {
            return false;
        }
    }
    return true;
}

int AIBid::getNumStrongCards() const
{
    const auto &hand = spades.getHand(spades.getTurnSeat());
    return std::count_if(hand.begin(), hand.end(), [this](const auto &card)
                         { return analyze.isStrongStartCard(card); });
}

int AIBid::getBid() const
{
    const auto possibleBids = spades.getPossibleBids();
    assert(!possibleBids.empty());

    if (possibleBids.size() == 1 || hasWeakHand())
    {
        return possibleBids.front();
    }
    else
    {
        int targetBid = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        targetBid += getNumStrongCards() / 3;

        const auto teamBid = spades.getBidResult(SeatUtils::getTeamSeat(spades.getTurnSeat()));
        targetBid -= teamBid.value_or(0);
        
        return getClosestNonZeroBid(targetBid);
    }
}

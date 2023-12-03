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
    return false;
}

int AIBid::getBid() const
{
    const auto possibleBids = spades.getPossibleBids();
    assert(!possibleBids.empty());

    if (possibleBids.size() == 1)
    {
        return possibleBids.front();
    }
    else if (hasWeakHand())
    {
        return possibleBids.front();
    }
    else
    {
        const int guaranteedTakes = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        int targetBid = guaranteedTakes;
        return getClosestNonZeroBid(targetBid);
    }
}

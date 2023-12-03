#include "spades_ai/AIBid.h"
#include "spades_ai/Analyze.h"

using namespace spd;

int AIBid::getClosestNonZeroBid(const std::vector<int> possibleBids, int target)
{
    int bid = possibleBids.front();
    for (const auto possibleBid : possibleBids)
    {
        if (bid == 0 && possibleBid != 0)
        {
            bid = possibleBid;
        }
        else if (abs(possibleBid - target) < abs(bid - target))
        {
            bid = possibleBid;
        }
    }
    return bid;
}

int AIBid::getBid(const Spades &spades)
{
    Analyze analyze(spades);
    const auto possibleBids = spades.getPossibleBids();
    const int guaranteedTakes = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());

    if (guaranteedTakes > 0)
    {
        return getClosestNonZeroBid(possibleBids, guaranteedTakes);
    }

    return possibleBids.front();
}

#include "spades_ai/AIBid.h"

using namespace spd;

int AIBid::getBid(const Spades &spades)
{
    const auto possibleBids = spades.getPossibleBids();

    return possibleBids.front();
}

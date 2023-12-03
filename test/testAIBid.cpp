#include "test_pch.h"
#include "spades/Spades.h"
#include "spades_ai/Analyze.h"
#include "spades_ai/AIBid.h"

using namespace spd;

TEST(AIBIdding, ReasonableBid)
{
    Spades spades;
    Analyze analyze(spades);

    for (int i = 0; i < NUM_SEATS; i++)
    {
        const int bid = AIBid::getBid(spades);
        const int guaranteedWins = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        const auto possibleBids = spades.getPossibleBids(spades.getTurnSeat());
        if (possibleBids.size() > 0 && guaranteedWins > 0)
        {
            ASSERT_TRUE(bid != 0);
        }
        spades.addBid(bid);
    }
}
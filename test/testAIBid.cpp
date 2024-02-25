#include "test_pch.h"
#include "spades/Spades.h"
#include "spades_ai/logic/Analyze.h"
#include "spades_ai/logic/AIBid.h"
#include "spades_ai/AIController.h"

using namespace spd;

TEST(AIBIdding, ReasonableBid)
{
    Spades spades;
    Analyze analyze(spades);
    AIBid aiBid(spades);

    for (int i = 0; i < NUM_SEATS; i++)
    {
        const int bid = aiBid.getBid();
        const int guaranteedWins = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        const auto possibleBids = spades.getPossibleBids(spades.getTurnSeat());
        if (possibleBids.size() > 0 && guaranteedWins > 0)
        {
            ASSERT_TRUE(bid != 0);
        }
        spades.addBid(bid);
    }
}
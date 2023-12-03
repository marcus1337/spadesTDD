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
        int bid = AIBid::getBid(spades);
        int guaranteedWins = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        ASSERT_GE(bid, guaranteedWins);
        spades.addBid(bid);
    }
}
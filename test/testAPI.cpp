#include "test_pch.h"
#include "spades/Spades.h"

using namespace spd;

TEST(API, SetBidVariation) {
    Spades spades;
    const auto bidType = BidVariationType::DOUBLE_NILL;
    spades.reset(bidType, TrumpVariationType::ACE_HIGH);
    EXPECT_TRUE(spades.getBidVariationType() == bidType);
}

TEST(API, SetTrumpVariation) {
    Spades spades;
    const auto trumpType = TrumpVariationType::ACE_HIGH;
    spades.reset(trumpType);
    EXPECT_TRUE(spades.getTrumpVariationType() == trumpType);
}

TEST(API, BidOrderRound1) {
    Spades spades;
    for(const auto& seat : SeatUtils::getSeats()){
        EXPECT_FALSE(spades.hasBid(seat));
    }
    EXPECT_TRUE(spades.getTurnSeat() == Seat::SOUTH);
    EXPECT_FALSE(spades.hasBid(Seat::SOUTH));
    spades.addBid(1);
    EXPECT_TRUE(spades.getTurnSeat() == Seat::WEST);
    EXPECT_TRUE(spades.hasBid(Seat::SOUTH));
    spades.addBid(1);
    EXPECT_TRUE(spades.getTurnSeat() == Seat::NORTH);
    EXPECT_TRUE(spades.hasBid(Seat::WEST));
    spades.addBid(1);
    EXPECT_TRUE(spades.getTurnSeat() == Seat::EAST);
    EXPECT_TRUE(spades.hasBid(Seat::NORTH));
}

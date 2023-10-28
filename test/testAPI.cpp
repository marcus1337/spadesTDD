#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>

using namespace spd;

TEST(API, SetBidVariation) {
    Spades spades;
    const auto type = BidVariationType::DOUBLE_NILL;
    spades.setBidVariation(type);
    EXPECT_TRUE(spades.getBidVariationType() == type);
}

TEST(API, SetTrumpVariation) {
    Spades spades;
    const auto type = TrumpVariationType::ACE_HIGH;
    spades.setTrumpVariation(type);
    EXPECT_TRUE(spades.getTrumpVariationType() == type);
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

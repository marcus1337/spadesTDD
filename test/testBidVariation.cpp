#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include "table/Deck.h"
#include <set>
#include <cassert>
#include <algorithm>
#include <ranges>

using namespace spd;

TEST(BidVariation, BlindOption)
{
    Spades spades;
    const Seat seat = Seat::SOUTH;
    std::vector<BidOption> bidOptions = spades.getBidOptions(seat);
    bool canBlindBid = std::ranges::find(bidOptions, BidOption::SHOW_HAND) != bidOptions.end();
    EXPECT_TRUE(canBlindBid);
    spades.setBidOption(seat, BidOption::SHOW_HAND);
    bidOptions = spades.getBidOptions(seat);
    canBlindBid = std::ranges::find(bidOptions, BidOption::SHOW_HAND) != bidOptions.end();
    EXPECT_FALSE(canBlindBid);
}

#include "rules/BidVariation.h"

#include <cassert>
#include <optional>
#include <memory>
#include <string>
#include <optional>
#include <memory>
#include <map>
#include <cassert>

using namespace spd;

static constexpr int MAX_BID = 13;

std::vector<int> BidVariation::getAdjustedBidsBasedOnTeam(const Seat &seat, const State &state) const
{
    int teamBid = 0;
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    if (state.hasBid(teamSeat))
    {
        teamBid += state.getBid(teamSeat);
    }
    std::vector<int> bids;
    for (int i = 0; i <= MAX_BID - teamBid; i++)
        bids.push_back(i);
    return bids;
}

std::vector<int> BidVariation::getBidEqualToSpades(const Seat &seat, const State &state) const
{
    const auto &hand = state.getHand(seat);
    const int numSpades = std::count_if(hand.begin(), hand.end(),
                                        [](const auto &card)
                                        { return card.is(Suit::SPADE); });
    return {numSpades};
}

std::optional<int> BidVariation::getStandardBidResult(const Seat &seat, const State &state) const
{
    if (state.hasBid(seat))
        return std::make_optional(state.getBid(seat));
    else
        return std::nullopt;
}

std::vector<int> DoubleBlindNil::getBids(const Seat &seat, const State &state) const
{
    return getAdjustedBidsBasedOnTeam(seat, state);
}

std::vector<BidOption> DoubleBlindNil::getBidOptions(const Seat &seat, const State &state) const
{
    if (!state.hasBidOption(seat, BidOption::SHOW_HAND))
    {
        return {BidOption::SHOW_HAND};
    }
    else
    {
        return {};
    }
}

std::optional<int> DoubleBlindNil::getBidResult(const Seat &seat, const State &state) const
{
    return getStandardBidResult(seat, state);
}

std::vector<int> DoubleNil::getBids(const Seat &seat, const State &state) const
{
    return getAdjustedBidsBasedOnTeam(seat, state);
}
std::vector<BidOption> DoubleNil::getBidOptions(const Seat &seat, const State &state) const
{
    return {};
}
std::optional<int> DoubleNil::getBidResult(const Seat &seat, const State &state) const
{
    return getStandardBidResult(seat, state);
}

std::vector<int> Mirror::getBids(const Seat &seat, const State &state) const
{
    return getBidEqualToSpades(seat, state);
}
std::vector<BidOption> Mirror::getBidOptions(const Seat &seat, const State &state) const
{
    return {};
}
std::optional<int> Mirror::getBidResult(const Seat &seat, const State &state) const
{
    return getStandardBidResult(seat, state);
}

std::vector<int> Suicide::getPossibleNonNilBids() const
{
    std::vector<int> bids;
    for (int i = 4; i <= MAX_BID; i++)
    {
        bids.push_back(i);
    }
    return bids;
}

std::vector<int> Suicide::getPossibleFirstBids() const
{
    std::vector<int> bids;
    bids.push_back(0);
    for (const int bid : getPossibleNonNilBids())
    {
        bids.push_back(bid);
    }
    return bids;
}

std::vector<int> Suicide::getPossibleSecondaryBids(const Seat &seat, const State &state) const
{
    std::vector<int> bids;
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    if (state.getBid(teamSeat) != 0)
    {
        bids.push_back(0);
        bids.push_back(state.getBid(teamSeat));
    }
    else
    {
        for (const int bid : getPossibleNonNilBids())
        {
            bids.push_back(bid);
        }
    }
    return bids;
}

bool Suicide::isFirstBidderInTeam(const Seat &seat, const State &state) const
{
    const auto startBidSeat = SeatUtils::getStartBidSeat(state.getRound());
    const auto secondBidSeat = SeatUtils::getNextSeat(startBidSeat);
    return seat == startBidSeat || seat == secondBidSeat;
}

std::vector<int> Suicide::getBids(const Seat &seat, const State &state) const
{
    if (state.hasBid(SeatUtils::getTeamSeat(seat)))
        return getPossibleSecondaryBids(seat, state);
    else
        return getPossibleFirstBids();
}

std::vector<BidOption> Suicide::getBidOptions(const Seat &seat, const State &state) const
{
    return {};
}

std::optional<int> Suicide::getBidResult(const Seat &seat, const State &state) const
{
    const auto teamSeat = SeatUtils::getTeamSeat(seat);
    if (state.hasBid(seat) && state.hasBid(teamSeat))
    {
        if (isFirstBidderInTeam(seat, state))
        {
            if (state.getBid(seat) != 0 && state.getBid(teamSeat) != 0)
            {
                return std::make_optional(0);
            }
        }
    }
    return getStandardBidResult(seat, state);
}

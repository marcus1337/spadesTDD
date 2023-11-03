#include "data/State.h"
#include <cassert>
#include <algorithm>
#include <iterator>

using namespace spd;

std::vector<int> State::getMadeRoundBids() const
{
    std::vector<int> madeRoundBids;
    const int fromIndex = getRound() * SeatUtils::numSeats;
    const int toIndex = fromIndex + SeatUtils::numSeats;
    for (int i = fromIndex; i < toIndex && i < bids.size(); i++)
    {
        madeRoundBids.push_back(bids[i]);
    }
    return madeRoundBids;
}

std::array<Seat, SeatUtils::numSeats> State::getRoundBidOrder() const
{
    std::array<Seat, SeatUtils::numSeats> bidOrder{};
    const Seat startSeat = SeatUtils::getStartBidSeat(getRound());
    const auto seatOrder = SeatUtils::getSeatOrder(startSeat, SeatUtils::numSeats);
    std::copy(seatOrder.begin(), seatOrder.end(), bidOrder.begin());
    return bidOrder;
}

std::array<std::optional<std::pair<Seat, int>>, SeatUtils::numSeats> State::getRoundBids() const
{
    std::array<std::optional<std::pair<Seat, int>>, SeatUtils::numSeats> roundBids{};
    const auto roundBidOrder = getRoundBidOrder();
    const auto madeRoundBids = getMadeRoundBids();
    for (int i = 0; i < madeRoundBids.size(); i++)
    {
        roundBids[i] = std::make_optional(std::make_pair(roundBidOrder[i], madeRoundBids[i]));
    }
    return roundBids;
}

void State::setSeed(unsigned int seed)
{
    deck.setSeed(seed);
}

unsigned int State::getSeed() const
{
    return deck.getSeed();
}

std::array<std::pair<Seat, Card>, SeatUtils::numSeats> State::getTrick() const
{
    std::array<std::pair<Seat, Card>, SeatUtils::numSeats> trick;
    const int offset = trickTakers.size() * SeatUtils::numSeats;
    for (int i = offset; i < SeatUtils::numSeats + offset; i++)
    {
        trick[i] = playedCards[i];
    }
    return trick;
}

std::vector<std::pair<Seat, Card>> State::getPlayedTrickCardSeatPairs() const
{
    std::vector<std::pair<Seat, Card>> trickCards;
    const int offset = trickTakers.size() * SeatUtils::numSeats;
    for (int i = offset; i < SeatUtils::numSeats + offset; i++)
    {
        trickCards.push_back(playedCards[i]);
    }
    return trickCards;
}

std::vector<std::pair<Seat, Card>> State::getPlayedCards(int round) const
{
    std::vector<std::pair<Seat, Card>> roundCards;
    const int numCardsPerRound = 13;
    const int fromIndex = round * numCardsPerRound;
    for (int i = fromIndex; i < numCardsPerRound && i < playedCards.size(); i++)
    {
        roundCards.push_back(playedCards[i]);
    }
    return roundCards;
}

void State::clear()
{
    bids.clear();
    roundBidOptions.clear();
    playedCards.clear();
}

int State::getRound() const
{
    const int cardsPerRound = 52;
    return playedCards.size() % cardsPerRound;
}

bool State::isBidPhase() const
{
    return bids.size() < (getRound() + 1) * SeatUtils::numSeats;
}

bool State::hasBidOption(const Seat &seat, const BidOption &bidOption) const
{
    if (!roundBidOptions.contains(getRound()))
        return false;
    return roundBidOptions.at(getRound()).contains(std::make_pair(seat, bidOption));
}

void State::setBidOption(const Seat &seat, const BidOption &bidOption)
{
    if (!roundBidOptions.contains(getRound()))
        roundBidOptions[getRound()] = std::set<std::pair<Seat, BidOption>>();
    roundBidOptions[getRound()].emplace(std::make_pair(seat, bidOption));
}

bool State::hasBid(const Seat &seat) const
{
    const auto roundBids = getRoundBids();
    return std::any_of(roundBids.begin(), roundBids.end(),
                       [&seat](const auto &roundBid)
                       {
                           return roundBid.has_value() && roundBid->first == seat;
                       });
}

int State::getBid(const Seat &seat) const
{
    const auto roundBids = getRoundBids();
    auto it = std::find_if(roundBids.begin(), roundBids.end(),
                           [&seat](const auto &roundBid)
                           {
                               return roundBid.has_value() && roundBid->first == seat;
                           });
    assert(it != roundBids.end());
    return (*it)->second;
}

bool State::hasGameStarted() const
{
    return !bids.empty();
}

std::vector<Card> State::getHand(const Seat &seat) const
{
    auto startHand = deck.getHand(seat, getRound());
    auto playedRoundCards = getPlayedCards(getRound());
    std::vector<Card> hand;
    for (const auto &card : startHand)
    {
        if (!std::any_of(playedRoundCards.begin(), playedRoundCards.end(),
                         [&](const auto &playedSeatCardPair)
                         { return playedSeatCardPair.second == card; }))
        {
            hand.push_back(card);
        }
    }
    return hand;
}

void State::playCard(const Seat &seat, const Card &card)
{
    playedCards.push_back(std::make_pair(seat, card));
}
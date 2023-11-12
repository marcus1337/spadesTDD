#include "data/State.h"
#include <cassert>
#include <algorithm>
#include <iterator>

using namespace spd;

std::vector<int> State::getRoundBidValues() const
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

std::vector<std::pair<Seat, int>> State::getRoundBids() const
{
    std::vector<std::pair<Seat, int>> roundBids;
    const auto roundBidOrder = getRoundBidOrder();
    const auto roundBidValues = getRoundBidValues();
    for (int i = 0; i < roundBidValues.size(); i++)
    {
        roundBids.push_back(std::make_pair(roundBidOrder[i], roundBidValues[i]));
    }
    return roundBids;
}

std::vector<std::pair<Seat, int>> State::getBids() const
{
    std::vector<std::pair<Seat, int>> allBids;
    for (const int bid : bids)
    {
        const int round = allBids.size() / SeatUtils::numSeats;
        const auto startSeat = SeatUtils::getStartBidSeat(round);
        const auto seatIndex = (((int)startSeat) + allBids.size()) % SeatUtils::numSeats;
        const auto seat = (Seat)seatIndex;
        allBids.push_back(std::make_pair(seat, bid));
    }
    return allBids;
}

std::vector<std::array<std::pair<Seat, int>, SeatUtils::numSeats>> State::getCompletedRoundBids() const
{
    std::vector<std::array<std::pair<Seat, int>, SeatUtils::numSeats>> completedRoundBids;
    const auto allBids = getBids();
    for (int i = 0; i < getRound(); i++)
    {
        std::array<std::pair<Seat, int>, SeatUtils::numSeats> roundBids;
        for (int j = 0; j < SeatUtils::numSeats; j++)
        {
            roundBids[j] = allBids[j + i * SeatUtils::numSeats];
        }
        completedRoundBids.push_back(roundBids);
    }
    return completedRoundBids;
}

std::vector<std::set<std::pair<Seat, BidOption>>> State::getCompletedRoundBidOptions() const
{
    std::vector<std::set<std::pair<Seat, BidOption>>> comepletedRoundBidOptions;
    for (int i = 0; i < getRound(); i++)
    {
        std::set<std::pair<Seat, BidOption>> bidOptions;
        if (roundBidOptions.contains(i))
        {
            bidOptions = roundBidOptions.at(i);
        }
        comepletedRoundBidOptions.push_back(bidOptions);
    }
    return comepletedRoundBidOptions;
}

std::vector<std::array<std::pair<Seat, Card>, SeatUtils::numSeats>> State::getTricks() const
{
    std::vector<std::array<std::pair<Seat, Card>, SeatUtils::numSeats>> tricks;
    const int numTricks = playedSeatCardPairs.size() / SeatUtils::numSeats;
    for (int i = 0; i < numTricks; i++)
    {
        std::array<std::pair<Seat, Card>, SeatUtils::numSeats> trick;
        for (int j = 0; j < SeatUtils::numSeats; j++)
        {
            trick[j] = playedSeatCardPairs[j + (i * SeatUtils::numSeats)];
        }
        tricks.push_back(trick);
    }
    return tricks;
}

std::vector<std::pair<Seat, Card>> State::getPlayedTrickCardSeatPairs() const
{
    std::vector<std::pair<Seat, Card>> trickCards;
    const int offset = playedSeatCardPairs.size() / SeatUtils::numSeats;
    for (int i = offset; i < SeatUtils::numSeats + offset && i < playedSeatCardPairs.size(); i++)
    {
        trickCards.push_back(playedSeatCardPairs[i]);
    }
    return trickCards;
}

std::vector<Card> State::getPlayedTrickCards() const
{
    std::vector<Card> playedTrickCards;
    for (const auto &[seat, card] : getPlayedTrickCardSeatPairs())
    {
        playedTrickCards.push_back(card);
    }
    return playedTrickCards;
}

std::vector<std::pair<Seat, Card>> State::getPlayedCardSeatPairs(int round) const
{
    std::vector<std::pair<Seat, Card>> roundCards;
    const int numCardsPerRound = 52;
    const int fromIndex = round * numCardsPerRound;
    for (int i = fromIndex; i < fromIndex + numCardsPerRound && i < playedSeatCardPairs.size(); i++)
    {
        roundCards.push_back(playedSeatCardPairs[i]);
    }
    return roundCards;
}

std::vector<std::pair<Seat, Card>> State::getAllPlayedSeatCardPairs() const
{
    return playedSeatCardPairs;
}

std::vector<Card> State::getPlayedCards(int round) const
{
    std::vector<Card> cards;
    for (const auto &[seat, card] : getPlayedCardSeatPairs(round))
    {
        cards.push_back(card);
    }
    return cards;
}

std::vector<Card> State::getPlayedRoundCards() const
{
    return getPlayedCards(getRound());
}

void State::clear()
{
    bids.clear();
    roundBidOptions.clear();
    playedSeatCardPairs.clear();
}

int State::getRound() const
{
    const int cardsPerRound = 52;
    return playedSeatCardPairs.size() / cardsPerRound;
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
                           return roundBid.first == seat;
                       });
}

int State::getBid(const Seat &seat) const
{
    const auto roundBids = getRoundBids();
    auto it = std::find_if(roundBids.begin(), roundBids.end(),
                           [&seat](const auto &roundBid)
                           {
                               return roundBid.first == seat;
                           });
    assert(it != roundBids.end());
    return (*it).second;
}

bool State::hasGameStarted() const
{
    return !bids.empty();
}

void State::playCard(const Seat &seat, const Card &card)
{
    playedSeatCardPairs.push_back(std::make_pair(seat, card));
}

void State::addBid(int bid)
{
    bids.push_back(bid);
}
void State::popBid()
{
    bids.pop_back();
}
void State::popCard()
{
    playedSeatCardPairs.pop_back();
}

void State::removeBidOption(const Seat &seat, const BidOption &bidOption)
{
    if (roundBidOptions.contains(getRound()))
    {
        roundBidOptions[getRound()].erase(std::make_pair(seat, bidOption));
    }
}

Seat State::getTurn(const Seat &trickStartSeat) const
{
    const int round = getRound();
    const auto playedRoundSeatCardPairs = getPlayedCardSeatPairs(round);
    const int startBidIndex = round % SeatUtils::numSeats;

    if (isBidPhase())
    {
        int playerIndex = (bids.size() + startBidIndex) % SeatUtils::numSeats;
        return (Seat)playerIndex;
    }
    else if (playedRoundSeatCardPairs.size() < SeatUtils::numSeats)
    {
        return (Seat)playedRoundSeatCardPairs.size();
    }
    else
    {
        const int numTrickSteps = getPlayedTrickCardSeatPairs().size();
        return SeatUtils::getNextSeats(trickStartSeat, numTrickSteps).back();
    }
}

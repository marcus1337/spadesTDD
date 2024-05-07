#include "spades/data/State.h"
#include "spades/rules/Constants.h"
#include <cassert>
#include <algorithm>
#include <iterator>

using namespace spd;

std::vector<int> State::getRoundBidValues(int round) const
{
    std::vector<int> madeRoundBids;
    const int fromIndex = round * NUM_SEATS;
    const int toIndex = fromIndex + NUM_SEATS;
    for (int i = fromIndex; i < toIndex && i < bids.size(); i++)
    {
        madeRoundBids.push_back(bids[i]);
    }
    return madeRoundBids;
}

std::array<Seat, NUM_SEATS> State::getRoundBidOrder(int round) const
{
    std::array<Seat, NUM_SEATS> bidOrder{};
    const Seat startSeat = SeatUtils::getStartBidSeat(round);
    const auto seatOrder = SeatUtils::getSeatOrder(startSeat, NUM_SEATS);
    std::copy(seatOrder.begin(), seatOrder.end(), bidOrder.begin());
    return bidOrder;
}

std::vector<std::pair<Seat, int>> State::getRoundBids(int round) const
{
    std::vector<std::pair<Seat, int>> roundBids;
    const auto roundBidOrder = getRoundBidOrder(round);
    const auto roundBidValues = getRoundBidValues(round);
    for (int i = 0; i < roundBidValues.size(); i++)
    {
        roundBids.push_back(std::make_pair(roundBidOrder[i], roundBidValues[i]));
    }
    return roundBids;
}

std::vector<std::pair<Seat, int>> State::getRoundBids() const
{
    return getRoundBids(getRound());
}

std::vector<std::pair<Seat, int>> State::getBids() const
{
    std::vector<std::pair<Seat, int>> allBids;
    for (const int bid : bids)
    {
        const int round = allBids.size() / NUM_SEATS;
        const auto startSeat = SeatUtils::getStartBidSeat(round);
        const auto seatIndex = (((int)startSeat) + allBids.size()) % NUM_SEATS;
        const auto seat = (Seat)seatIndex;
        allBids.push_back(std::make_pair(seat, bid));
    }
    return allBids;
}

std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> State::getCompletedRoundBids() const
{
    return getCompletedRoundBids(getRound());
}

std::vector<std::set<std::pair<Seat, BidOption>>> State::getCompletedRoundBidOptions() const
{
    return getCompletedRoundBidOptions(getRound());
}

std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> State::getCompletedRoundBids(int targetRound) const
{
    std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> completedRoundBids;
    const auto allBids = getBids();
    for (int i = 0; i < targetRound; i++)
    {
        std::array<std::pair<Seat, int>, NUM_SEATS> roundBids;
        for (int j = 0; j < NUM_SEATS; j++)
        {
            roundBids[j] = allBids[j + i * NUM_SEATS];
        }
        completedRoundBids.push_back(roundBids);
    }
    return completedRoundBids;
}

std::vector<std::set<std::pair<Seat, BidOption>>> State::getCompletedRoundBidOptions(int targetRound) const
{
    std::vector<std::set<std::pair<Seat, BidOption>>> comepletedRoundBidOptions;
    for (int i = 0; i < targetRound; i++)
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

std::set<std::pair<Seat, BidOption>> State::getRoundBidOptions(int targetRound) const
{
    if (roundBidOptions.contains(targetRound))
    {
        return roundBidOptions.at(targetRound);
    }
    else
    {
        return {};
    }
}

std::vector<std::array<std::pair<Seat, Card>, NUM_SEATS>> State::getTricks() const
{
    return getTricks(0);
}

std::vector<std::array<std::pair<Seat, Card>, NUM_SEATS>> State::getTricks(std::size_t fromIndex) const
{
    std::vector<std::array<std::pair<Seat, Card>, NUM_SEATS>> tricks;
    const std::size_t numTricks = playedSeatCardPairs.size() / NUM_SEATS;

    if (fromIndex < numTricks)
    {
        tricks.reserve(numTricks - fromIndex);
        auto it = playedSeatCardPairs.begin() + fromIndex * NUM_SEATS;

        for (std::size_t i = fromIndex; i < numTricks; ++i)
        {
            std::array<std::pair<Seat, Card>, NUM_SEATS> trick;
            for (auto &seatCardPair : trick)
            {
                seatCardPair = *it++;
            }
            tricks.push_back(trick);
        }
    }

    return tricks;
}

std::vector<std::pair<Seat, Card>> State::getCurrentTrickCardSeatPairs() const
{
    std::vector<std::pair<Seat, Card>> trickCards;
    trickCards.reserve(NUM_SEATS);
    const std::size_t startIndex = (playedSeatCardPairs.size() / NUM_SEATS) * NUM_SEATS;
    trickCards.insert(trickCards.end(), playedSeatCardPairs.begin() + startIndex, playedSeatCardPairs.end());
    return trickCards;
}

std::vector<Card> State::getCurrentTrickCards() const
{
    std::vector<Card> playedTrickCards;
    for (const auto &[seat, card] : getCurrentTrickCardSeatPairs())
    {
        playedTrickCards.push_back(card);
    }
    return playedTrickCards;
}

std::vector<std::pair<Seat, Card>> State::getPlayedCardSeatPairs(int round) const
{
    std::vector<std::pair<Seat, Card>> roundCards;
    const int fromIndex = round * DECK_SIZE;
    for (int i = fromIndex; i < fromIndex + DECK_SIZE && i < playedSeatCardPairs.size(); i++)
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
    return playedSeatCardPairs.size() / DECK_SIZE;
}

bool State::isBidPhase() const
{
    return bids.size() < (getRound() + 1) * NUM_SEATS;
}

bool State::hasBidOption(const Seat &seat, const BidOption &bidOption, int targetRound) const
{
    if (!roundBidOptions.contains(targetRound))
        return false;
    return roundBidOptions.at(targetRound).contains(std::make_pair(seat, bidOption));
}

bool State::hasBidOption(const Seat &seat, const BidOption &bidOption) const
{
    return hasBidOption(seat, bidOption, getRound());
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

Seat State::getTrickTurn(const Seat &startSeat) const
{
    const auto &currentTrickCardSeatPairs = getCurrentTrickCardSeatPairs();
    const int numTrickSteps = currentTrickCardSeatPairs.size();
    const auto seat = (Seat)(((int)startSeat + numTrickSteps) % NUM_SEATS);
    assert(numTrickSteps < NUM_SEATS);
    for (const auto &cardSeatPair : currentTrickCardSeatPairs)
    {
        assert(cardSeatPair.first != seat);
    }

    return seat;
}

Seat State::getBidTurn() const
{
    return (Seat)((bids.size() + getRound()) % NUM_SEATS);
}

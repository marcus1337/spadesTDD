#include "data/memento/SpadesMemento.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <limits>
#include "data/memento/json.h"

using namespace spd;

static constexpr const char *BID_VAR_KEY = "BidVariationType";
static constexpr const char *TRUMP_VAR_KEY = "TrumpVariationType";
static constexpr const char *SEED_KEY = "seed";
static constexpr const char *BIDS_KEY = "bids";
static constexpr const char *HISTORY_KEY = "history";
SpadesMemento::SpadesMemento() : bidVariationType(0), trumpVariationType(0), seed(0)
{
}
SpadesMemento::SpadesMemento(const std::string &data)
{
    deserialize(data);
}

BidVariationType SpadesMemento::getBidVariationType() const
{
    if (bidVariationType < (unsigned int)BidVariationType::LAST)
        return (BidVariationType)bidVariationType;
    else
        return BidVariationType{};
}

TrumpVariationType SpadesMemento::getTrumpVariationType() const
{
    if (trumpVariationType < (unsigned int)TrumpVariationType::LAST)
        return (TrumpVariationType)trumpVariationType;
    else
        return TrumpVariationType{};
}

std::string SpadesMemento::serialize() const
{
    nlohmann::json j;
    j[BID_VAR_KEY] = bidVariationType;
    j[TRUMP_VAR_KEY] = trumpVariationType;
    j[SEED_KEY] = seed;
    for (const auto &bid : bids)
    {
        j[BIDS_KEY].push_back(bid);
    }
    j[HISTORY_KEY] = historyEncoding;
    return j.dump();
}

void SpadesMemento::deserialize(const std::string &data)
{
    nlohmann::json j = nlohmann::json::parse(data);
    bidVariationType = j[BID_VAR_KEY];
    trumpVariationType = j[TRUMP_VAR_KEY];
    seed = j[SEED_KEY];
    bids.clear();
    if (j.find(BIDS_KEY) != j.end() && j[BIDS_KEY].is_array())
    {
        for (const auto &element : j[BIDS_KEY])
        {
            bids.push_back(element);
        }
    }
    historyEncoding = j[HISTORY_KEY];
}

std::vector<std::pair<Seat, Card>> SpadesMemento::getPlayedSeatCardPairs() const
{
    std::vector<std::pair<Seat, Card>> playedSeatCardPairs;
    for (int i = 0; i + 1 < playedSeatCardPairData.size(); i += 2)
    {
        unsigned int seatValue = playedSeatCardPairData[i];
        unsigned int cardValue = playedSeatCardPairData[i + 1];
        Seat seat = (Seat)(seatValue % 4);
        Card card(cardValue);
        playedSeatCardPairs.push_back(std::make_pair(seat, card));
    }

    return playedSeatCardPairs;
}

std::map<int, std::set<std::pair<Seat, BidOption>>> SpadesMemento::getRoundBidOptions() const
{
    std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;
    for (int i = 0; i + 2 < roundBidOptionsData.size(); i += 3)
    {
        unsigned int roundIndex = roundBidOptionsData[i];
        unsigned int seatValue = roundBidOptionsData[i + 1];
        unsigned int bidOptionValue = roundBidOptionsData[i + 2];
        Seat seat = (Seat)(seatValue % 4);
        BidOption bidOption = (BidOption)(bidOptionValue % ((int)BidOption::LAST));
        if (!roundBidOptions.contains(roundIndex))
        {
            roundBidOptions[i] = std::set<std::pair<Seat, BidOption>>();
        }
        roundBidOptions[i].insert(std::make_pair(seat, bidOption));
    }
    return roundBidOptions;
}

std::vector<Seat> SpadesMemento::getTrickTakers() const
{
    std::vector<Seat> trickTakers;
    for (const unsigned int seatValue : trickTakersData)
    {
        Seat seat = (Seat)(seatValue % 4);
        trickTakers.push_back(seat);
    }
    return trickTakers;
}

void SpadesMemento::setPlayedSeatCardPairsData(const std::vector<std::pair<Seat, Card>> &playedSeatCardPairs)
{
    playedSeatCardPairData.clear();
    for (const auto &pair : playedSeatCardPairs)
    {
        playedSeatCardPairData.push_back((unsigned int)pair.first);
        playedSeatCardPairData.push_back(pair.second.serialize());
    }
}

void SpadesMemento::setRoundBidOptionsData(const std::map<int, std::set<std::pair<Seat, BidOption>>> &roundBidOptions)
{
    roundBidOptionsData.clear();
    for (const auto &[key, value] : roundBidOptions)
    {
        for (const auto &pair : value)
        {
            roundBidOptionsData.push_back(key);
            roundBidOptionsData.push_back((unsigned int)pair.first);
            roundBidOptionsData.push_back((unsigned int)pair.second);
        }
    }
}

void SpadesMemento::setTrickTakersData(const std::vector<Seat> &seats)
{
    trickTakersData.clear();
    for (const auto &seat : seats)
    {
        trickTakersData.push_back((unsigned int)seat);
    }
}

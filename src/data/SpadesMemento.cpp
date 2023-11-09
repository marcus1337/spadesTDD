#include "data/memento/SpadesMemento.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <limits>
#include "data/memento/json.h"

using namespace spd;

static constexpr int UNSET_VALUE = std::numeric_limits<int>::min();
static constexpr const char *BID_VAR_KEY = "BidVariationType";
static constexpr const char *TRUMP_VAR_KEY = "TrumpVariationType";
static constexpr const char *SEED_KEY = "seed";
static constexpr const char *BIDS_KEY = "bids";
static constexpr const char *HISTORY_KEY = "history";
SpadesMemento::SpadesMemento() : bidVariationType(UNSET_VALUE), trumpVariationType(UNSET_VALUE), seed(UNSET_VALUE)
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
    assert(bidVariationType != UNSET_VALUE);
    assert(trumpVariationType != UNSET_VALUE);
    assert(seed != UNSET_VALUE);

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

    return playedSeatCardPairs;
}
std::map<int, std::set<std::pair<Seat, BidOption>>> SpadesMemento::getRoundBidOptions() const
{
    std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;
    return roundBidOptions;
}
std::vector<Seat> SpadesMemento::getTrickTakers() const
{
    std::vector<Seat> trickTakers;
    
    return trickTakers;
}

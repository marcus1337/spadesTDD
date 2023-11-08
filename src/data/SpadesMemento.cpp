#include "data/memento/SpadesMemento.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <limits>
#include "data/memento/json.h"

using namespace spd;

static constexpr int UNSET_VALUE = std::numeric_limits<int>::min();
static constexpr const char *bidVariationTypeKey = "BidVariationType";
static constexpr const char *trumpVariationTypeKey = "TrumpVariationType";

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
    j[bidVariationTypeKey] = bidVariationType;
    j[trumpVariationTypeKey] = trumpVariationType;
    j["seed"] = seed;
    for (const auto &bid : bids)
    {
        j["bids"].push_back(bid);
    }
    j["history"] = historyEncoding;
    return j.dump();
}

void SpadesMemento::deserialize(const std::string &data)
{
    nlohmann::json j = nlohmann::json::parse(data);
    bidVariationType = j[bidVariationTypeKey];
    trumpVariationType = j[trumpVariationTypeKey];
    seed = j["seed"];
    bids.clear();
    if (j.find("bids") != j.end() && j["bids"].is_array())
    {
        for (const auto &element : j["bids"])
        {
            bids.push_back(element);
        }
    }
    historyEncoding = j["history"];
}

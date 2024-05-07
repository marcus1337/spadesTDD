#include "spades/data/Move.h"
#include "spades/data/memento/json.h"

using namespace spd;

Move::Move(const Seat &seat, const Card &card) : seat(seat), card(card) {}

Move::Move(const Seat &seat, int bid) : seat(seat), bid(bid) {}

Move::Move(const Seat &seat, const BidOption &bidOption) : seat(seat), bidOption(bidOption) {}

std::string Move::serialize() const
{
    using namespace nlohmann;
    json j;
    if (card.has_value())
    {
        j["card"] = card.value().serialize();
    }
    if (bid.has_value())
    {
        j["bid"] = bid.value();
    }
    if (bidOption.has_value())
    {
        j["bidOption"] = static_cast<unsigned int>(bidOption.value());
    }
    j["seat"] = static_cast<unsigned int>(seat);
    return j.dump();
}
bool Move::deserialize(const std::string &encoding)
{
    card = deserializeCard(encoding);
    bid = deserializeBid(encoding);
    bidOption = deserializeBidOption(encoding);
    const auto tmpSeat = deserializeSeat(encoding);
    return tmpSeat.has_value() && (card.has_value() || bid.has_value() || bidOption.has_value());
}

std::optional<spd::Card> Move::deserializeCard(const std::string &encoding) const
{
    auto value = loadUIntKey(encoding, CARD_KEY);
    if (value.has_value())
    {
        return Card(value.value());
    }
    else
    {
        return std::nullopt;
    }
}
std::optional<unsigned int> Move::deserializeBid(const std::string &encoding) const
{
    auto value = loadUIntKey(encoding, BID_KEY);
    if (value.has_value())
    {
        value = std::clamp<unsigned int>(value.value(), 0, 13);
    }
    return value;
}
std::optional<spd::BidOption> Move::deserializeBidOption(const std::string &encoding) const
{
    auto value = loadUIntKey(encoding, CARD_KEY);
    if (value.has_value())
    {
        const unsigned int MAX_VALUE = ((int)BidOption::LAST) - 1;
        value = std::clamp<unsigned int>(value.value(), 0, MAX_VALUE);
        return (BidOption)value.value();
    }
    return std::nullopt;
}
std::optional<spd::Seat> Move::deserializeSeat(const std::string &encoding) const
{
    auto value = loadUIntKey(encoding, CARD_KEY);
    if (value.has_value())
    {
        value = std::clamp<unsigned int>(value.value(), 0, 3);
        return std::make_optional<Seat>((Seat)value.value());
    }
    return std::nullopt;
}

std::optional<unsigned int> Move::loadUIntKey(const std::string &encoding, const std::string &key) const
{
    using namespace nlohmann;
    const auto &data = json::parse(encoding);
    if (data.contains(key) && data[key].is_number_unsigned())
    {
        return std::make_optional<unsigned int>(data[key].get<unsigned int>());
    }
    return std::nullopt;
}
#pragma once
#include "spades/data/Seat.h"
#include "spades/data/Bid.h"
#include "spades/data/card/Card.h"
#include "spades/data/Serializable.h"
#include <optional>

namespace spd
{
    class Move : public Serializable
    {
    public:
        Move(const std::string &encoding);
        Move(const Seat &seat, const Card &card);
        Move(const Seat &seat, int bid);
        Move(const Seat &seat, const BidOption &bidOption);
        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &encoding) override;

        std::optional<spd::Card> card;
        std::optional<unsigned int> bid;
        std::optional<spd::BidOption> bidOption;
        spd::Seat seat = spd::Seat::SOUTH;

    private:
        static constexpr const char* BID_KEY = "M_BID";
        static constexpr const char* BIDOPT_KEY = "M_BIDOPT";
        static constexpr const char* CARD_KEY = "M_CARD";
        static constexpr const char* SEAT_KEY = "M_SEAT";
        std::optional<spd::Card> deserializeCard(const std::string &encoding) const;
        std::optional<unsigned int> deserializeBid(const std::string &encoding) const;
        std::optional<spd::BidOption> deserializeBidOption(const std::string &encoding) const;
        std::optional<spd::Seat> deserializeSeat(const std::string &encoding) const;

        std::optional<unsigned int> loadUIntKey(const std::string &encoding, const std::string &key) const;
    };
}

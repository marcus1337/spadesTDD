
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include "data/card/Card.h"
#include "data/Seat.h"

namespace spd
{
    class TrumpVariation
    {
    protected:
        std::optional<Card> getLeadCard(const std::array<std::pair<Seat, Card>, SeatUtils::numSeats> &trick) const
        {
            for (const auto &cardSeatPair : trick)
            {
                if (!cardSeatPair.second.is(Joker::BIG) && !cardSeatPair.second.is(Joker::LITTLE))
                {
                    return std::make_optional(cardSeatPair.second);
                }
            }
            return std::nullopt;
        }
        std::optional<Suit> getLeadSuit(const std::array<std::pair<Seat, Card>, SeatUtils::numSeats> &trick) const
        {
            const auto leadCard = getLeadCard(trick);
            for (const auto suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART, Suit::SPADE})
                if (leadCard.has_value() && leadCard.value().is(suit))
                    return std::make_optional(suit);
            return std::nullopt;
        }

        virtual bool isTrumpCard(const Card& card) const = 0;
        virtual int compareRank(const Card& cardA, const Card& cardB) = 0;

    public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
        virtual Seat getTrickTaker(const std::array<std::pair<Seat, Card>, SeatUtils::numSeats> &trick) const{

            return Seat::SOUTH;
        }
    };

    class AceHigh : public TrumpVariation
    {

    public:
        AceHigh() = default;
        virtual bool isTrumpCard(const Card& card) const override{
            return card.is(Suit::SPADE);
        }

        virtual int compareRank(const Card& cardA, const Card& cardB) override {
            return 0;
        }
    };
}

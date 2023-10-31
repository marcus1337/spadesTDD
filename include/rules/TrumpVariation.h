
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
        std::optional<Card> getLeadCard(const State &state) const
        {
            for (const auto &cardSeatPair : state.getTrick())
            {
                if (!cardSeatPair.second.is(Joker::BIG) && !cardSeatPair.second.is(Joker::LITTLE))
                {
                    return std::make_optional(cardSeatPair.second);
                }
            }
            return std::nullopt;
        }
        std::optional<Suit> getLeadSuit(const State &state) const
        {
            const auto leadCard = getLeadCard(state);
            for (const auto suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART, Suit::SPADE})
                if (leadCard.has_value() && leadCard.value().is(suit))
                    return std::make_optional(suit);
            return std::nullopt;
        }

        virtual bool isTrumpCard(const Card &card) const = 0;
        virtual int getCardValue(const Card &card) const = 0;
        bool isLeadSuit(const Card& card, const State& state) const {
            const auto leadSuit = getLeadSuit(state);
            if(leadSuit.has_value()){
                const Suit suit = leadSuit.value();
                return card.is(leadSuit.value());
            }
            return false;
        }

    public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
        virtual Seat getTrickTaker(const State &state) const
        {
            const auto trick = state.getTrick();
            Card winCard = trick.front().second;
            Seat winSeat = trick.front().first;
            for(const auto& cardSeatPair : trick){
                const Seat seat = cardSeatPair.first;
                const Card card = cardSeatPair.second;
                if(!isTrumpCard(winCard) && isTrumpCard(card)){
                    winSeat = seat;
                    winCard = card;
                }else if (!isTrumpCard(winCard) && !isTrumpCard(card) && !isLeadSuit(winCard, state) && isLeadSuit(card, state)){
                    winSeat = seat;
                    winCard = card;
                }else if(isTrumpCard(winCard) && isTrumpCard(card) && getCardValue(winCard) < getCardValue(card)){
                    winSeat = seat;
                    winCard = card;
                } else if (!isTrumpCard(winCard) && !isTrumpCard(card) && getCardValue(winCard) < getCardValue(card)){
                    winSeat = seat;
                    winCard = card;
                }
            }

            return winSeat;
        }
    };

    class AceHigh : public TrumpVariation
    {

    public:
        AceHigh() = default;
        virtual bool isTrumpCard(const Card &card) const override
        {
            return card.is(Suit::SPADE);
        }

        virtual int getCardValue(const Card &card) const override {
            using enum Rank;
            const auto ranks = NormalCardValue::getRanks();
            for(int i = 1; i < ranks.size(); i++){
                if(card.is(ranks[i])){
                    return i-1;
                }
            }
            if(card.is(Rank::ACE))
            {
                return ranks.size();
            }
            return 0;
        }
    };
}

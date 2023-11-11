
#pragma once
#include <vector>
#include <optional>
#include "data/card/Card.h"
#include "data/Seat.h"
#include "data/State.h"
#include "TrumpVariation.h"

namespace spd
{

    struct TrickCardComparator
    {
        bool trump;
        bool leadSuit;
        int value;
        bool operator<(const TrickCardComparator &other) const;
    };

    class Trick
    {

        const TrumpVariation &trumpVariation;
        const State &state;

        TrickCardComparator getTrickCardComparator(const Card &card) const;
        std::optional<Card> getLeadCard() const;
        std::optional<Suit> getLeadSuit() const;
        int getNormalValue(const Card& card) const;
        int getTrumpValue(const Card& card) const;
        int getValue(const Card &card) const;
        bool isLeadSuit(const Card &card) const;
        bool isNewTopCard(const Card &topCard, const Card &newCard) const;
        bool sameSuit(const Card &card1, const Card &card2) const;
        bool canPlaceFirst(const Card &card, const Seat& seat) const;
        bool canPlaceContinuation(const Card& card, const Seat& seat) const;
        bool hasSameSuit(const Card& card, const Seat& seat) const;
        bool hasTrumpBeenPlayed() const;
        bool hasOnlyTrumpCards(const Seat& seat) const;

    public:
        Trick(const TrumpVariation &trumpVariation, const State &state);
        bool canPlace(const Seat& turnSeat, const Card &card) const;
        Seat getTrickTaker() const;
    };
}

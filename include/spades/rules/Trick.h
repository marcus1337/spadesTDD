
#pragma once
#include <vector>
#include <optional>
#include "spades/data/card/Card.h"
#include "spades/data/Seat.h"
#include "spades/data/State.h"
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
        std::array<std::pair<Seat, Card>, NUM_SEATS> trick;

        TrickCardComparator getTrickCardComparator(const Card &card) const;
        std::optional<Suit> getLeadSuit() const;
        int getValue(const Card &card) const;
        bool isLeadSuit(const Card &card) const;
        bool isNewTopCard(const Card &topCard, const Card &newCard) const;
        bool sameSuit(const Card &card1, const Card &card2) const;
        bool hasSameSuit(const Card& card, const std::vector<Card>& hand) const;
        Card getLeadCard() const;
    public:
        Trick(const TrumpVariation &trumpVariation, const State &state, const std::array<std::pair<Seat, Card>, NUM_SEATS>& trick);
        Seat getTrickTaker() const;

    };
}

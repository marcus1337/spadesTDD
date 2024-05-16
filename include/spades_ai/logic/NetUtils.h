#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    namespace net
    {
        bool wasFirstSpadePlacedStartOfTrick(const Spades &spades);
        bool hasStartedTrickBreakingSpades(const Spades &spades, const Seat &seat);
        bool hasSkippedLeadSuit(const Spades &spades, const Suit &leadSuit, const Seat &seat);
        bool hasSkippedLeadSuit(const Spades &spades, const std::vector<std::pair<Seat, Card>> &csPairs, int from, const Suit &suit, const Seat &seat);
        bool areAllSuitCardsPlaced(const Spades &spades, const Suit &suit);
        bool hasActiveNilBid(const Spades &spades, const Seat &seat);
        Seat getRelativeSeatPosition(const Spades &spades, const Seat &relSeat);
        unsigned int getRelativeSeatIndex(const Spades &spades, const Seat &relSeat);
        unsigned int getCardIndex(const Spades &spades, const Card &card);
        unsigned int getSuitIndex(const Spades &spades, const Card &card);
        unsigned int getRankIndex(const Spades &spades, const Card &card);

    }

}
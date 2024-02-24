#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

namespace spd
{
    class AIObservation
    {
        const Spades &spades;

        // TODO: remove?
        bool isLastTrickCard() const;
        bool canPlaceTopCard(const Seat &seat) const;
        bool canPlaceNonTopCard(const Seat &seat) const;
        bool isNilBidAtRisk(const Seat &seat) const;
        bool isDefendingNil(const Seat &seat) const;
        bool canSabotageOpponentNil() const;
        bool canDefendTeamNil() const;
        bool teamNeedTricks() const;
        bool opponentTeamNeedTricks() const;
        //-TODO: remove?

        bool hasSkippedLeadSuit(const Suit& leadSuit, const Seat& seat) const;
        bool sameSuit(const Card& card1, const Card& card2) const;
        bool hasSuit(const Suit& suit, const Card& card) const;
        std::vector<Card> filterCards(const Suit& suit, const std::vector<Card>& cards) const;

    public:
        AIObservation(const Spades &spades);

        std::vector<float> getNetInput() const;
    };
}
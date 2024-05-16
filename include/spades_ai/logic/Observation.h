#pragma once
#include "spades/Spades.h"
#include "spades/pch.h"

// Observation assumens each suit has 13 ranks, if more ranks due to special rules crop out selection to top 13 ranks and assume lower ranks are the "lowest rank".

/*
Large-network: (13 cards per suit - if more than 13 crop out lower ranked cards)
52 bool - placed cards
52 bool - start cards
52 bool - currently owned cards
4 bool - lead card suit
13 bool - lead card rank
13 bool - need 1-13 more tricks?
3 bool - top card is placed by relative other seat
3 bool - placed trick cards
(12) 4 * 3 bool - seat is known to not have suit

Optional: 3+1 bool - has relative seat active nil-bid?
Optional: 13 opponent needs 1-13 more tricks.
Optional: 13 opponent team has total bid 1-13
*/

namespace spd
{
    class Observation
    {
        static constexpr auto OBSERVATION_SIZE = DECK_SIZE + DECK_SIZE + DECK_SIZE + 4 + 13 + (NUM_SEATS - 1) * 3 + NUM_SEATS + 13 + 13;
        std::array<float, OBSERVATION_SIZE> observation;
        Seat seat, teamSeat, oppSeat1, oppSeat2;

        std::array<float, DECK_SIZE> getPlacedCards(const Spades &spades) const;
        std::array<float, DECK_SIZE> getStartCards(const Spades &spades) const;
        std::array<float, DECK_SIZE> getCurrentlyOwnedCards(const Spades &spades) const;
        std::array<float, 4> getLeadCardSuit(const Spades &spades) const;
        bool isTopCardTrump(const Spades &spades) const;
        std::array<float, 13> getTopCardRank(const Spades &spades) const;
        std::array<float, NUM_SEATS - 1> getTopCardSeat(const Spades &spades) const;           // Relative seat positions
        std::array<float, NUM_SEATS - 1> getPlacedTrickCardSeats(const Spades &spades) const;   // Relative seat positions
        std::array<float, 4 * (NUM_SEATS - 1)> getKnownAbsentSuits(const Spades &spades) const; // Relative seat positions
        std::array<float, NUM_SEATS> getActiveNilBidSeats(const Spades &spades) const;          // Relative seat positions
        std::array<float, 13> getNumNeededTricksOpponent(const Spades &spades) const;
        std::array<float, 13> getNumNeededTricksTeam(const Spades &spades) const;

    public:
        Observation(const Spades &spades);
        const std::array<float, OBSERVATION_SIZE> &getValues() const;
    };
}
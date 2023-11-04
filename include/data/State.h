#pragma once
#include <vector>
#include <string>
#include <optional>
#include <map>
#include <set>
#include "data/card/Card.h"
#include "table/Deck.h"

namespace spd
{
    class State
    {
        Deck deck;
        std::vector<int> getMadeRoundBids() const;
        std::array<Seat, SeatUtils::numSeats> getRoundBidOrder() const;
        std::array<std::optional<std::pair<Seat, int>>, SeatUtils::numSeats> getRoundBids() const;

    public:

        void setSeed(unsigned int seed);
        unsigned int getSeed() const;

        std::array<std::pair<Seat, Card>, SeatUtils::numSeats> getTrick() const;
        std::vector<std::pair<Seat, Card>> getPlayedTrickCardSeatPairs() const;
        std::vector<Card> getPlayedTrickCards() const;        
        std::vector<std::pair<Seat, Card>> getPlayedCardSeatPairs(int round) const;
        std::vector<Card> getPlayedCards(int round) const;
        State() = default;

        void clear();
        int getRound() const;
        bool isBidPhase() const;
        bool hasBidOption(const Seat &seat, const BidOption &bidOption) const;
        void setBidOption(const Seat &seat, const BidOption &bidOption);
        bool hasBid(const Seat &seat) const;
        int getBid(const Seat &seat) const;
        bool hasGameStarted() const;
        std::vector<Card> getHand(const Seat &seat) const;
        void playCard(const Seat &seat, const Card &card);

        std::vector<int> bids;
        std::vector<std::pair<Seat, Card>> playedSeatCardPairs;
        std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;
        std::vector<Seat> trickTakers;
    };
}
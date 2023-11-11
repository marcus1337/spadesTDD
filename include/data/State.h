#pragma once
#include <vector>
#include <string>
#include <optional>
#include <map>
#include <set>
#include "data/card/Card.h"
#include "rules/Deck.h"

namespace spd
{
    class State
    {
        std::array<Seat, SeatUtils::numSeats> getRoundBidOrder() const;
        std::vector<int> getRoundBidValues() const;
    public:
        std::vector<std::pair<Seat, int>> getRoundBids() const;
        std::vector<std::array<std::pair<Seat, Card>, SeatUtils::numSeats>> getTricks() const;
        std::vector<std::pair<Seat, Card>> getPlayedTrickCardSeatPairs() const;
        std::vector<Card> getPlayedTrickCards() const;        
        std::vector<std::pair<Seat, Card>> getPlayedCardSeatPairs(int round) const;
        std::vector<Card> getPlayedCards(int round) const;
        std::vector<Card> getPlayedRoundCards() const;
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
        void playCard(const Seat& seat, const Card &card);
        void addBid(int bid);
        void popBid();
        void removeBidOption(const Seat &seat, const BidOption &bidOption);
        Seat getTurn(const Seat& trickStartSeat) const;

        Deck deck;
        std::vector<int> bids;
        std::vector<std::pair<Seat, Card>> playedSeatCardPairs;
        std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;
    };
}
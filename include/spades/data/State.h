#pragma once
#include <vector>
#include <string>
#include <optional>
#include <map>
#include <set>
#include "spades/data/card/Card.h"
#include "spades/data/Seat.h"
#include "spades/data/SeatUtils.h"

namespace spd
{
    class State
    {
        std::array<Seat, NUM_SEATS> getRoundBidOrder(int round) const;
        std::vector<int> getRoundBidValues(int round) const;

        std::vector<int> bids;
        std::vector<std::pair<Seat, Card>> playedSeatCardPairs;
        std::map<int, std::set<std::pair<Seat, BidOption>>> roundBidOptions;

    public:
        std::vector<std::pair<Seat, int>> getBids() const;
        std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> getCompletedRoundBids() const;
        std::vector<std::set<std::pair<Seat, BidOption>>> getCompletedRoundBidOptions() const;
        std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> getCompletedRoundBids(int targetRound) const;
        std::vector<std::set<std::pair<Seat, BidOption>>> getCompletedRoundBidOptions(int targetRound) const;
        std::vector<std::pair<Seat, int>> getRoundBids() const;
        std::vector<std::pair<Seat, int>> getRoundBids(int round) const;
        std::vector<std::array<std::pair<Seat, Card>, NUM_SEATS>> getTricks() const;
        std::vector<std::array<std::pair<Seat, Card>, NUM_SEATS>> getTricks(std::size_t fromIndex) const;
        std::vector<std::pair<Seat, Card>> getCurrentTrickCardSeatPairs() const;
        std::vector<Card> getCurrentTrickCards() const;
        std::vector<std::pair<Seat, Card>> getAllPlayedSeatCardPairs() const;
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
        void playCard(const Seat &seat, const Card &card);
        void addBid(int bid);
        void popBid();
        void popCard();
        void removeBidOption(const Seat &seat, const BidOption &bidOption);
        Seat getTrickTurn(const Seat &startSeat) const;
        Seat getBidTurn() const;
    };
}
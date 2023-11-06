#pragma once
#include <vector>
#include <string>
#include "rules/BidVariationController.h"
#include "rules/TrumpVariationController.h"
#include "data/memento/SpadesMemento.h"
#include "data/State.h"
#include "rules/Turn.h"
#include "data/Bid.h"
#include "data/Score.h"

namespace spd
{
    class Spades
    {
        State state;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;
        Turn turn;

        SpadesMemento makeMemento() const;
        void loadMemento(const SpadesMemento &memento);
        void setBidVariation(BidVariationType type);
        void setTrumpVariation(TrumpVariationType type);
        void setSeed(unsigned int seed);

    public:
        Spades();
        void reset(int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType);
        void reset(BidVariationType bidVariationType, TrumpVariationType trumpVariationType);
        void reset(BidVariationType bidVariationType);
        void reset(TrumpVariationType trumpVariationType);
        void reset(unsigned int seed);
        void reset();
        void resetAndRandomizeSeed();
        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
        Score getScore() const;
        unsigned int getSeed() const;
        Seat getTurnSeat() const;
        void addBid(int bid);
        bool hasBid(const Seat &seat) const;
        bool isBidPhase() const;
        std::vector<int> getPossibleBids(const Seat &seat) const;
        std::vector<int> getPossibleBids() const;
        std::vector<BidOption> getBidOptions(const Seat &seat) const;
        std::vector<Card> getHand(const Seat& seat) const;
        void setBidOption(const Seat &seat, const BidOption &bidOption);
        std::optional<int> getBidResult(const Seat& seat) const;
        void playCard(const Card& card);
        bool canPlayCard(const Card& card) const;

        std::vector<Card> getTrumpCardsDescending() const;
        std::array<Card,2> getExcludedCards() const;

        void undo();
        bool canUndo() const;

    };
}
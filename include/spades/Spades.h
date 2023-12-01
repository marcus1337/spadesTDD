#pragma once
#include <vector>
#include <string>
#include "spades/history/SpadesHistory.h"
#include "spades/rules/BidVariationController.h"
#include "spades/rules/TrumpVariationController.h"
#include "spades/rules/Deck.h"
#include "spades/data/memento/SpadesMemento.h"
#include "spades/data/State.h"
#include "spades/data/Bid.h"
#include "spades/data/Score.h"

namespace spd
{
    class Spades
    {
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;
        Deck deck;
        SpadesHistory history;
        State state;

        SpadesMemento createMemento() const;
        void loadMemento(const SpadesMemento &memento);
        void setBidVariation(BidVariationType type);
        void setTrumpVariation(TrumpVariationType type);
        void setSeed(unsigned int seed);

        bool hasCorruptCards() const;
        bool hasCorruptBids() const;

        std::vector<std::vector<Seat>> getCompletedRoundTrickTakers() const;
    public:
        Spades() = default;
        void reset(int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType);
        void reset(BidVariationType bidVariationType, TrumpVariationType trumpVariationType);
        void reset(BidVariationType bidVariationType);
        void reset(TrumpVariationType trumpVariationType);
        void reset(unsigned int seed);
        void reset();
        void resetAndRandomizeSeed();

        BidVariationType getBidVariationType() const;
        TrumpVariationType getTrumpVariationType() const;
        unsigned int getSeed() const;
        Seat getTurnSeat() const;
        std::pair<Score, Score> getScore() const;

        bool hasBid(const Seat &seat) const;
        bool isBidPhase() const;
        bool canPlace(const Card &card) const;

        std::vector<int> getPossibleBids(const Seat &seat) const;
        std::vector<int> getPossibleBids() const;
        std::vector<BidOption> getBidOptions(const Seat &seat) const;
        std::vector<Card> getHand(const Seat &seat) const;
        std::optional<int> getBidResult(const Seat &seat) const;
        std::vector<Card> getTrumpCardsDescending() const;
        std::array<Card, 2> getExcludedCards() const;
        std::vector<std::pair<Seat, Card>> getCurrentTrickCardSeatPairs() const;

        void place(const Card &card);
        void addBid(unsigned int bid);
        void setBidOption(const Seat &seat, const BidOption &bidOption);

        bool isCorrupt() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
        void undo();
        bool canUndo() const;
        void redo();
        bool canRedo() const;
    };
}
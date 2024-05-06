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
#include "spades/rules/ScoreSettings.h"

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
        std::vector<Seat> getTrickTakers(int round) const;

        std::vector<Card> getStartCards() const;

    public:
        ScoreSettings scoreSettings;
        Spades() = default;
        Spades(const Spades &other);
        void reset(unsigned int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType);
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
        bool isTurnSeat(const Seat &seat) const;
        std::pair<Score, Score> getScore() const;

        bool hasBid(const Seat &seat) const;
        bool isBidPhase() const;
        bool canPlace(const Card &card) const;
        bool canSetBidOption(const Seat& seat, const BidOption& bidOption) const;
        bool canSeeHand(const Seat &seat) const;

        std::vector<int> getPossibleBids(const Seat &seat) const;
        std::vector<int> getPossibleBids() const;
        std::vector<BidOption> getBidOptions(const Seat &seat) const;
        std::vector<Card> getUnplacedRoundCards() const;
        std::vector<Card> getRoundStartCards(const Seat &seat) const;
        std::vector<Card> getHand(const Seat &seat) const;
        std::vector<Card> getPlaceableCards(const Seat &card) const;
        std::vector<Card> getPlaceableCards() const;
        std::optional<int> getBidResult(const Seat &seat) const;
        std::vector<Card> getTrumpCardsDescending() const;
        std::vector<Rank> getRanksDescending() const;
        std::array<Card, NUM_EXCLUDED_CARDS> getExcludedCards() const;
        std::optional<Seat> getCurrentTrickTopSeat() const;

        std::optional<Seat> getPrevTrickTaker() const;
        std::vector<std::pair<Seat, Card>> getPrevTrickCardSeatPairs() const;

        int getCountedRoundTricks(const Seat &seat) const;
        std::vector<std::pair<Seat, Card>> getCurrentTrickCardSeatPairs() const;
        std::vector<std::pair<Seat, Card>> getCurrentRoundCardSeatPairs() const;
        bool isSpadesBroken() const;
        int getNumberOfTakenTricksCurrentRound(const Seat &seat) const;

        Suit getEffectiveSuit(const Card &card) const;
        bool isTrumpCard(const Card &card) const;
        void place(const Card &card);
        void addBid(unsigned int bid);
        void setBidOption(const Seat &seat, const BidOption &bidOption);
        bool hasBidOption(const Seat &seat, const BidOption &bidOption) const;

        bool isCorrupt() const;
        std::string serialize() const;
        void deserialize(const std::string &data);
        void undo();
        bool canUndo() const;
        void redo();
        bool canRedo() const;

        bool isTopCardIfPlaced(const Card &card) const;
        int getCardStrengthRelativeToCurrentTrick(const Card &card) const;
        int getCardStrength(const Card &card) const;
        int getCardStrengthRelativeToLeadSuit(const Suit &leadSuit, const Card &card) const;
    };
}
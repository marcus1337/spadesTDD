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
#include "spades/data/Move.h"

namespace std
{
    template <>
    struct hash<std::pair<spd::Seat, int>>
    {
        std::size_t operator()(const std::pair<spd::Seat, int> &p) const noexcept
        {
            return std::hash<spd::Seat>{}(p.first) ^ std::hash<int>{}(p.second);
        }
    };
}

namespace spd
{
    class Spades
    {
        Deck deck;
        SpadesHistory history;
        State state;
        TrumpVariationController trumpVarController;
        BidVariationController bidVarController;

        mutable std::unordered_map<std::pair<Seat, int>, std::vector<Card>> cachedStartHands; // Temporary values used for optimization

        SpadesMemento createMemento() const;
        void loadMemento(const SpadesMemento &memento);
        void setBidVariation(BidVariationType type);
        void setTrumpVariation(TrumpVariationType type);
        void setSeed(unsigned int seed);

        bool hasCorruptCards() const;
        bool hasCorruptBids() const;

        std::vector<std::vector<Seat>> getCompletedRoundTrickTakers() const;
        std::vector<std::vector<Seat>> getCompletedRoundTrickTakers(int targetRound) const;
        std::vector<Seat> getTrickTakers(int round) const;

        Score getTeamScore(const std::pair<Seat, Seat> &team) const;
        Score getTeamScore(const std::pair<Seat, Seat> &team, int targetRound) const;

        GameMode getMode() const;
        bool canPlace(const Card &card, const std::vector<Card>& hand) const;

    public:
        ScoreSettings scoreSettings;
        Spades() = default;
        Spades(const Spades &other);
        Spades(const std::string &encoding);
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
        std::pair<Score, Score> getScore(int targetRound) const;
        int getRound() const;

        bool hasBid(const Seat &seat) const;
        bool isBidPhase() const;
        bool canPlace(const Card &card) const;
        bool canSetBidOption(const Seat &seat, const BidOption &bidOption) const;
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
        std::optional<int> getBidResult(const Seat &seat, int round) const;
        std::vector<Card> getTrumpCardsDescending() const;
        std::vector<Rank> getRanksDescending() const;
        std::array<Card, NUM_EXCLUDED_CARDS> getExcludedCards() const;
        std::optional<Seat> getCurrentTrickTopSeat() const;
        std::optional<Seat> getSpadeBreaker(int round) const;
        std::vector<Card> getStartCards() const;

        std::optional<Seat> getPrevTrickTaker() const;
        std::vector<std::pair<Seat, Card>> getPrevTrickCardSeatPairs() const;

        int getCountedRoundTricks(const Seat &seat) const;
        std::vector<std::pair<Seat, Card>> getCurrentTrickCardSeatPairs() const;
        std::vector<std::pair<Seat, Card>> getCurrentRoundCardSeatPairs() const;
        bool isSpadesBroken() const;
        int getNumberOfTakenTricksCurrentRound(const Seat &seat) const;
        int getNumberOfTakenTricks(const Seat &seat, int targetRound) const;

        Suit getEffectiveSuit(const Card &card) const;
        bool isTrumpCard(const Card &card) const;
        void place(const Card &card);
        void addBid(unsigned int bid);
        void setBidOption(const Seat &seat, const BidOption &bidOption);
        bool hasBidOption(const Seat &seat, const BidOption &bidOption) const;
        bool hasBidOption(const Seat &seat, const BidOption &bidOption, int round) const;

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

        ScoreResult getScoreResult() const;
    };
}
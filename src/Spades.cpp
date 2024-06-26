#include "spades/Spades.h"
#include <cassert>
#include "spades/rules/Deck.h"

using namespace spd;

Spades::Spades(const std::string &encoding)
{
    deserialize(encoding);
}

Spades::Spades(const Spades &other)
{
    cachedStartHands.clear();
    deserialize(other.serialize());
}

SpadesMemento Spades::createMemento() const
{
    return SpadesMemento(history, scoreSettings, getMode());
}

GameMode Spades::getMode() const
{
    GameMode mode;
    mode.seed = getSeed();
    mode.bidVarType = getBidVariationType();
    mode.trumpVarType = getTrumpVariationType();
    return mode;
}

void Spades::loadMemento(const SpadesMemento &memento)
{
    scoreSettings = memento.getScoreSettings();
    history = memento.getHistory();
    state = history.load();
    const auto &mode = memento.getGameMode();
    deck.setSeed(mode.seed);
    bidVarController.setBidVariationType(mode.bidVarType);
    trumpVarController.setTrumpVariationType(mode.trumpVarType);
    deck.setExcludeCards(trumpVarController.getExcludedCards());
    scoreSettings.setBlindBonus(getBidVariationType() == BidVariationType::DOUBLE_BLIND_NILL);
}

void Spades::setBidVariation(BidVariationType type)
{
    cachedStartHands.clear();
    assert(!state.hasGameStarted());
    bidVarController.setBidVariationType(type);
}

void Spades::setTrumpVariation(TrumpVariationType type)
{
    cachedStartHands.clear();
    assert(!state.hasGameStarted());
    trumpVarController.setTrumpVariationType(type);
    deck.setExcludeCards(trumpVarController.getExcludedCards());
}

void Spades::setSeed(unsigned int seed)
{
    cachedStartHands.clear();
    assert(!state.hasGameStarted());
    deck.setSeed(seed);
}

void Spades::reset(unsigned int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType)
{
    cachedStartHands.clear();
    history.clear();
    state.clear();
    setSeed(seed);
    setBidVariation(bidVariationType);
    setTrumpVariation(trumpVariationType);
    deck.setExcludeCards(trumpVarController.getExcludedCards());
    scoreSettings.setBlindBonus(getBidVariationType() == BidVariationType::DOUBLE_BLIND_NILL);
}

void Spades::reset(BidVariationType bidVariationType, TrumpVariationType trumpVariationType)
{
    reset(getSeed(), bidVariationType, trumpVariationType);
}

void Spades::reset(BidVariationType bidVariationType)
{
    reset(getSeed(), bidVariationType, getTrumpVariationType());
}

void Spades::reset(TrumpVariationType trumpVariationType)
{
    reset(getSeed(), getBidVariationType(), trumpVariationType);
}

void Spades::reset(unsigned int seed)
{
    reset(seed, getBidVariationType(), getTrumpVariationType());
}

void Spades::reset()
{
    reset(getSeed());
}

void Spades::resetAndRandomizeSeed()
{
    reset(PortableRandom::makeRandomSeed());
}

BidVariationType Spades::getBidVariationType() const
{
    return bidVarController.getBidVariationType();
}

TrumpVariationType Spades::getTrumpVariationType() const
{
    return trumpVarController.getTrumpVariationType();
}

std::string Spades::serialize() const
{
    return createMemento().serialize();
}

void Spades::deserialize(const std::string &data)
{
    cachedStartHands.clear();
    loadMemento(SpadesMemento(data));
}

Score Spades::getTeamScore(const std::pair<Seat, Seat> &team) const
{
    return getTeamScore(team, getRound());
}

Score Spades::getTeamScore(const std::pair<Seat, Seat> &team, int targetRound) const
{
    return Score(scoreSettings, team, getCompletedRoundTrickTakers(targetRound), state.getCompletedRoundBids(targetRound), state.getCompletedRoundBidOptions(targetRound));
}

std::pair<Score, Score> Spades::getScore() const
{
    return getScore(getRound());
}

std::pair<Score, Score> Spades::getScore(int targetRound) const
{
    const auto team1 = std::make_pair(Seat::SOUTH, Seat::NORTH);
    const auto team2 = std::make_pair(Seat::WEST, Seat::EAST);
    return std::make_pair(getTeamScore(team1, targetRound), getTeamScore(team2, targetRound));
}

unsigned int Spades::getSeed() const
{
    return deck.getSeed();
}

Seat Spades::getTurnSeat() const
{
    if (state.isBidPhase())
    {
        return state.getBidTurn();
    }
    else
    {
        return state.getTrickTurn(trumpVarController.getTrickStartSeat(state));
    }
}

bool Spades::isTurnSeat(const Seat &seat) const
{
    return getTurnSeat() == seat;
}

int Spades::getRound() const
{
    return state.getRound();
}

void Spades::addBid(unsigned int bid)
{
    history.addAndApply(state, Move(getTurnSeat(), bid));
}

bool Spades::hasBid(const Seat &seat) const
{
    return state.hasBid(seat);
}

bool Spades::isBidPhase() const
{
    return state.isBidPhase();
}

bool Spades::canSeeHand(const Seat &seat) const
{
    if (isBidPhase())
    {
        for (const auto &bidOpt : getBidOptions(seat))
        {
            if (bidOpt == BidOption::SHOW_HAND)
            {
                return false;
            }
        }
    }
    return true;
}

bool Spades::canSetBidOption(const Seat &seat, const BidOption &bidOption) const
{
    if (isBidPhase() && !hasBidOption(seat, bidOption))
    {
        for (const auto &bidOpt : getBidOptions(seat))
        {
            if (bidOpt == bidOption)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<int> Spades::getPossibleBids(const Seat &seat) const
{
    return bidVarController.getBids(seat, state, getHand(seat));
}

std::vector<int> Spades::getPossibleBids() const
{
    return Spades::getPossibleBids(getTurnSeat());
}

std::vector<BidOption> Spades::getBidOptions(const Seat &seat) const
{
    return bidVarController.getBidOptions(seat, state);
}

std::vector<Card> Spades::getRoundStartCards(const Seat &seat) const
{
    const auto &key = std::make_pair(seat, getRound());
    if (!cachedStartHands.contains(key))
    {
        const auto &hand = deck.getHand(seat, state.getRound());
        cachedStartHands[key] = std::vector<Card>(hand.begin(), hand.end());
    }
    return cachedStartHands[key];
}

std::vector<Card> Spades::getHand(const Seat &seat) const
{
    std::set<Card> playedSet;
    for (const auto &pair : state.getPlayedCardSeatPairs(state.getRound()))
    {
        playedSet.insert(pair.second);
    }
    std::vector<Card> hand;
    for (const auto &card : getRoundStartCards(seat))
    {
        if (!playedSet.contains(card))
        {
            hand.push_back(card);
        }
    }
    return hand;
}

std::vector<Card> Spades::getPlaceableCards(const Seat &seat) const
{
    std::vector<Card> cards;
    const auto &hand = getHand(seat);
    for (const auto &card : hand)
    {
        if (canPlace(card, hand))
        {
            cards.push_back(card);
        }
    }
    return cards;
}

std::vector<Card> Spades::getPlaceableCards() const
{
    return getPlaceableCards(getTurnSeat());
}

void Spades::setBidOption(const Seat &seat, const BidOption &bidOption)
{
    history.addAndApply(state, Move(seat, bidOption));
}

bool Spades::hasBidOption(const Seat &seat, const BidOption &bidOption) const
{
    return hasBidOption(seat, bidOption, getRound());
}

bool Spades::hasBidOption(const Seat &seat, const BidOption &bidOption, int round) const
{
    return state.hasBidOption(seat, bidOption, round);
}

std::optional<int> Spades::getBidResult(const Seat &seat) const
{
    return bidVarController.getBidResult(seat, state);
}

std::optional<int> Spades::getBidResult(const Seat &seat, int round) const
{
    std::optional<int> result;
    if (round == getRound())
    {
        return getBidResult(seat);
    }
    else if (round >= 0 && round < getRound())
    {
        for (const auto &[key, value] : state.getRoundBids(round))
        {
            if (key == seat)
            {
                result = value;
            }
        }
    }
    return result;
}

void Spades::place(const Card &card)
{
    history.addAndApply(state, Move(getTurnSeat(), card));
}

bool Spades::canPlace(const Card &card) const
{
    return canPlace(card, getHand(getTurnSeat()));
}

bool Spades::canPlace(const Card &card, const std::vector<Card> &hand) const
{
    return trumpVarController.canPlaceCard(state, card, hand);
}

Suit Spades::getEffectiveSuit(const Card &card) const
{
    return trumpVarController.getEffectiveSuit(card);
}

bool Spades::isTrumpCard(const Card &card) const
{
    return getEffectiveSuit(card) == Suit::SPADE;
}

std::vector<Card> Spades::getTrumpCardsDescending() const
{
    return trumpVarController.getTrumpCardsOrderedByValueDescending();
}

std::vector<Rank> Spades::getRanksDescending() const
{
    std::vector<Rank> ranks;
    for (const auto &rank : Card::getRanks())
    {
        ranks.push_back(rank);
    }
    return ranks;
}

std::array<Card, NUM_EXCLUDED_CARDS> Spades::getExcludedCards() const
{
    return trumpVarController.getExcludedCards();
}

void Spades::undo()
{
    history.undo(state);
}

bool Spades::canUndo() const
{
    return history.canUndo();
}

void Spades::redo()
{
    history.redo(state);
}

bool Spades::canRedo() const
{
    return history.canRedo();
}

std::vector<std::pair<Seat, Card>> Spades::getCurrentTrickCardSeatPairs() const
{
    return state.getCurrentTrickCardSeatPairs();
}

int Spades::getCountedRoundTricks(const Seat &seat) const
{
    if (getBidResult(seat).value_or(0) == 0)
    {
        return 0;
    }
    else
    {
        return getNumberOfTakenTricksCurrentRound(seat);
    }
}

std::optional<Seat> Spades::getCurrentTrickTopSeat() const
{
    std::optional<Seat> topSeat = std::nullopt;
    int topValue = std::numeric_limits<int>::min();
    for (const auto &[seat, card] : getCurrentTrickCardSeatPairs())
    {
        int value = getCardStrengthRelativeToCurrentTrick(card);
        if (value > topValue)
        {
            value = topValue;
            topSeat = std::make_optional(seat);
        }
    }
    return topSeat;
}

std::vector<std::pair<Seat, Card>> Spades::getCurrentRoundCardSeatPairs() const
{
    return state.getPlayedCardSeatPairs(state.getRound());
}

bool Spades::isCorrupt() const
{
    return hasCorruptCards() || hasCorruptBids();
}

bool Spades::hasCorruptCards() const
{
    const int numBidSets = state.getCompletedRoundBids().size();
    const int maxPlayedCards = numBidSets * DECK_SIZE;
    bool corrupt = false;
    if (state.getAllPlayedSeatCardPairs().size() > maxPlayedCards)
    {
        corrupt = true;
    }
    for (int round = 0; round <= state.getRound(); round++)
    {
        std::set<Card> roundCards;
        for (const auto &card : state.getPlayedCards(round))
        {
            if (roundCards.contains(card))
            {
                corrupt = true;
            }
            roundCards.insert(card);
        }
    }
    return corrupt;
}

bool Spades::hasCorruptBids() const
{
    const int maxBid = HAND_SIZE;
    bool corrupt = false;
    const auto bids = state.getBids();
    for (const auto &bid : bids)
    {
        if (bid.second > maxBid)
        {
            corrupt = true;
        }
    }
    for (int i = 0; i + 2 < bids.size(); i++)
    {
        if (bids[i].second + bids[i + 2].second > maxBid)
        {
            corrupt = true;
        }
    }
    return corrupt;
}

std::vector<std::vector<Seat>> Spades::getCompletedRoundTrickTakers(int targetRound) const
{
    std::vector<std::vector<Seat>> roundTrickTakers;
    for (int round = 0; round < targetRound; round++)
    {
        roundTrickTakers.push_back(getTrickTakers(round));
    }
    return roundTrickTakers;
}

std::vector<std::vector<Seat>> Spades::getCompletedRoundTrickTakers() const
{
    return getCompletedRoundTrickTakers(getRound());
}

std::vector<Seat> Spades::getTrickTakers(int round) const
{
    const auto &trickTakers = trumpVarController.getTrickTakers(state);
    std::vector<Seat> roundTrickTakers;
    for (int i = HAND_SIZE * round; i < HAND_SIZE * round + HAND_SIZE && i < trickTakers.size(); i++)
    {
        roundTrickTakers.push_back(trickTakers[i]);
    }
    return roundTrickTakers;
}

int Spades::getNumberOfTakenTricksCurrentRound(const Seat &seat) const
{
    return getNumberOfTakenTricks(seat, state.getRound());
}

int Spades::getNumberOfTakenTricks(const Seat &seat, int targetRound) const
{
    int numTricks = 0;
    for (const auto &trickTaker : getTrickTakers(targetRound))
    {
        if (seat == trickTaker)
        {
            numTricks++;
        }
    }
    return numTricks;
}

bool Spades::isSpadesBroken() const
{
    for (const auto &pair : getCurrentRoundCardSeatPairs())
    {
        if (getEffectiveSuit(pair.second) == Suit::SPADE)
        {
            return true;
        }
    }
    return false;
}

int Spades::getCardStrengthRelativeToCurrentTrick(const Card &card) const
{
    const auto trickCards = getCurrentTrickCardSeatPairs();
    if (!trickCards.empty())
    {
        const Suit leadSuit = getEffectiveSuit(trickCards.front().second);
        return getCardStrengthRelativeToLeadSuit(leadSuit, card);
    }
    else
    {
        return getCardStrength(card);
    }
}

int Spades::getCardStrength(const Card &card) const
{
    const auto& trumpsDescending = getTrumpCardsDescending();
    const auto& ranksDescending = getRanksDescending();
    int trumpValue = trumpsDescending.size() + ranksDescending.size();
    for (const auto &trumpCard : trumpsDescending)
    {
        if (card == trumpCard)
        {
            return trumpValue;
        }
        trumpValue--;
    }
    int rankValue = ranksDescending.size();
    for (const auto &rank : ranksDescending)
    {
        if (card.is(rank))
        {
            return rankValue;
        }
        rankValue--;
    }
    return 0;
}
int Spades::getCardStrengthRelativeToLeadSuit(const Suit &leadSuit, const Card &card) const
{
    const auto suit = getEffectiveSuit(card);
    if (suit != leadSuit && suit != Suit::SPADE)
    {
        return 0;
    }
    return getCardStrength(card);
}

bool Spades::isTopCardIfPlaced(const Card &card) const
{
    const auto trickCardSeatPairs = getCurrentTrickCardSeatPairs();
    for (const auto &[seat, trickCard] : trickCardSeatPairs)
    {
        if (getCardStrengthRelativeToCurrentTrick(trickCard) > getCardStrengthRelativeToCurrentTrick(card))
        {
            return false;
        }
    }
    return true;
}

std::vector<Card> Spades::getStartCards() const
{
    std::vector<Card> cards;
    for (const auto &seat : SeatUtils::getSeats())
    {
        for (const auto &card : getRoundStartCards(seat))
        {
            cards.push_back(card);
        }
    }
    return cards;
}

std::vector<Card> Spades::getUnplacedRoundCards() const
{
    std::set<Card> placedCards;
    for (const auto &pair : state.getPlayedCardSeatPairs(state.getRound()))
    {
        placedCards.insert(pair.second);
    }

    std::vector<Card> cards;
    for (const auto &card : getStartCards())
    {
        if (!placedCards.contains(card))
        {
            cards.push_back(card);
        }
    }
    return cards;
}

std::optional<Seat> Spades::getPrevTrickTaker() const
{
    const auto &trickTakers = trumpVarController.getTrickTakers(state);
    if (trickTakers.empty())
        return std::nullopt;
    return trickTakers.back();
}

std::vector<std::pair<Seat, Card>> Spades::getPrevTrickCardSeatPairs() const
{
    const auto &tricks = state.getTricks();
    if (tricks.empty())
        return {};
    const auto &lastTrick = tricks.back();
    return std::vector<std::pair<Seat, Card>>(lastTrick.begin(), lastTrick.end());
}

std::optional<Seat> Spades::getSpadeBreaker(int round) const
{
    for (const auto &[key, value] : state.getPlayedCardSeatPairs(round))
    {
        if (isTrumpCard(value))
        {
            return key;
        }
    }
    return std::nullopt;
}

ScoreResult Spades::getScoreResult() const
{
    const auto &scores = getScore(getRound());
    return Score::getScoreResult(scoreSettings, scores.first, scores.second);
}

#include "spades/Spades.h"
#include <cassert>
#include "spades/history/SpadesCommand.h"
#include "spades/rules/Deck.h"

using namespace spd;

SpadesMemento Spades::createMemento() const
{
    return SpadesMemento(history.undoCommandContainer, history.redoCommandContainer, getTrumpVariationType(), getBidVariationType(), getSeed());
}

void Spades::loadMemento(const SpadesMemento &memento)
{
    reset(memento.getSeed(), memento.getBidVariationType(), memento.getTrumpVariationType());
    history.undoCommandContainer = memento.getUndoContainer();
    history.redoCommandContainer = memento.getRedoContainer();

    int undoBidIndex = 0;
    int undoCardIndex = 0;
    const auto &undoBids = history.undoCommandContainer.bidValueVariants;
    const auto &undoCards = history.undoCommandContainer.placeCommandValues;
    while (undoBidIndex < undoBids.size() || undoCardIndex < undoCards.size())
    {
        if (state.isBidPhase())
        {
            SpadesCommandValueVisitor::execute(undoBids[undoBidIndex++], state, trumpVariationController);
        }
        else
        {
            SpadesCommandValueVisitor::execute(undoCards[undoCardIndex++], state, trumpVariationController);
        }
    }
}

void Spades::setBidVariation(BidVariationType type)
{
    assert(!state.hasGameStarted());
    bidVariationController.setBidVariationType(type);
}

void Spades::setTrumpVariation(TrumpVariationType type)
{
    assert(!state.hasGameStarted());
    trumpVariationController.setTrumpVariationType(type);
    deck.setExcludeCards(trumpVariationController.getExcludedCards());
}

void Spades::setSeed(unsigned int seed)
{
    assert(!state.hasGameStarted());
    deck.setSeed(seed);
}

void Spades::reset(int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType)
{
    history.clear();
    state.clear();
    setSeed(seed);
    setBidVariation(bidVariationType);
    setTrumpVariation(trumpVariationType);
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
    return bidVariationController.getBidVariationType();
}

TrumpVariationType Spades::getTrumpVariationType() const
{
    return trumpVariationController.getTrumpVariationType();
}

std::string Spades::serialize() const
{
    return createMemento().serialize();
}

void Spades::deserialize(const std::string &data)
{
    loadMemento(SpadesMemento(data));
}

std::pair<Score, Score> Spades::getScore() const
{
    Score scoreSouthNorth({Seat::SOUTH, Seat::NORTH}, getCompletedRoundTrickTakers(), state.getCompletedRoundBids(), state.getCompletedRoundBidOptions());
    Score scoreWestEast({Seat::WEST, Seat::EAST}, getCompletedRoundTrickTakers(), state.getCompletedRoundBids(), state.getCompletedRoundBidOptions());
    return std::make_pair(scoreSouthNorth, scoreWestEast);
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
        return state.getTrickTurn(trumpVariationController.getTrickStartSeat(state));
    }
}

void Spades::addBid(unsigned int bid)
{
    history.addAndExecuteCommand(BidCommandValue{bid}, state, trumpVariationController);
}

bool Spades::hasBid(const Seat &seat) const
{
    return state.hasBid(seat);
}

bool Spades::isBidPhase() const
{
    return state.isBidPhase();
}

std::vector<int> Spades::getPossibleBids(const Seat &seat) const
{
    return bidVariationController.getBids(seat, state, getHand(seat));
}

std::vector<int> Spades::getPossibleBids() const
{
    return Spades::getPossibleBids(getTurnSeat());
}

std::vector<BidOption> Spades::getBidOptions(const Seat &seat) const
{
    return bidVariationController.getBidOptions(seat, state);
}

std::vector<Card> Spades::getHand(const Seat &seat) const
{
    auto startHand = deck.getHand(seat, state.getRound());
    auto playedRoundCards = state.getPlayedCardSeatPairs(state.getRound());
    std::vector<Card> hand;
    for (const auto &card : startHand)
    {
        if (!std::any_of(playedRoundCards.begin(), playedRoundCards.end(),
                         [&](const auto &playedSeatCardPair)
                         { return playedSeatCardPair.second == card; }))
        {
            hand.push_back(card);
        }
    }
    return hand;
}

std::vector<Card> Spades::getPlaceableCards(const Seat &seat) const
{
    std::vector<Card> cards;
    for (const auto &card : getHand(seat))
    {
        if (canPlace(card))
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
    history.addAndExecuteCommand(BidOptionCommandValue{bidOption, seat}, state, trumpVariationController);
}

std::optional<int> Spades::getBidResult(const Seat &seat) const
{
    return bidVariationController.getBidResult(seat, state);
}

void Spades::place(const Card &card)
{
    history.addAndExecuteCommand(PlaceCommandValue{card}, state, trumpVariationController);
}

bool Spades::canPlace(const Card &card) const
{
    return trumpVariationController.canPlaceCard(state, card, getHand(getTurnSeat()));
}

Suit Spades::getEffectiveSuit(const Card &card) const
{
    const auto trumpCards = getTrumpCardsDescending();
    if (std::find(trumpCards.begin(), trumpCards.end(), card) != trumpCards.end())
    {
        return Suit::SPADE;
    }
    else
    {
        return card.getSuit().value_or(Suit::SPADE);
    }
}

bool Spades::isTrumpCard(const Card &card) const
{
    return getEffectiveSuit(card) == Suit::SPADE;
}

std::vector<Card> Spades::getTrumpCardsDescending() const
{
    return trumpVariationController.getTrumpCardsOrderedByValueDescending();
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
    return trumpVariationController.getExcludedCards();
}

void Spades::undo()
{
    history.undo(state, trumpVariationController);
}

bool Spades::canUndo() const
{
    return history.canUndo();
}

void Spades::redo()
{
    history.redo(state, trumpVariationController);
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

std::vector<std::vector<Seat>> Spades::getCompletedRoundTrickTakers() const
{
    std::vector<std::vector<Seat>> roundTrickTakers;
    for (int round = 0; round < state.getRound(); round++)
    {
        roundTrickTakers.push_back(getTrickTakers(round));
    }
    return roundTrickTakers;
}

std::vector<Seat> Spades::getTrickTakers(int round) const
{
    const auto trickTakers = trumpVariationController.getTrickTakers(state);
    std::vector<Seat> roundTrickTakers;
    for (int i = HAND_SIZE * round; i < HAND_SIZE * round + HAND_SIZE && i < trickTakers.size(); i++)
    {
        roundTrickTakers.push_back(trickTakers[i]);
    }
    return roundTrickTakers;
}

int Spades::getNumberOfTakenTricksCurrentRound(const Seat &seat) const
{
    int numTricks = 0;
    for (const auto &trickTaker : getTrickTakers(state.getRound()))
    {
        if (seat == trickTaker)
        {
            numTricks++;
        }
    }
    return numTricks;
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
    const auto trumpsDescending = getTrumpCardsDescending();
    const auto ranksDescending = getRanksDescending();
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

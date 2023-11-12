#include "Spades.h"
#include <cassert>
#include "history/SpadesCommand.h"
#include "rules/Deck.h"

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
    const auto trickStartSeat = trumpVariationController.getTrickStartSeat(state);
    return state.getTurn(trickStartSeat);
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

std::vector<Card> Spades::getTrumpCardsDescending() const
{
    return trumpVariationController.getTrumpCardsOrderedByValueDescending();
}

std::array<Card, 2> Spades::getExcludedCards() const
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

std::vector<std::pair<Seat, Card>> Spades::getPlayedTrickSeatCardPairs() const
{
    return state.getPlayedTrickCardSeatPairs();
}

bool Spades::isCorrupt() const
{
    return hasCorruptCards() || hasCorruptBids();
}

bool Spades::hasCorruptCards() const
{
    const int cardsPerRound = 52;
    const int numBidSets = state.getCompletedRoundBids().size();
    const int maxPlayedCards = numBidSets * cardsPerRound;
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
    const int maxBid = 13;
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
    const auto trickTakers = trumpVariationController.getTrickTakers(state);
    const int tricksPerRound = 13;
    for (int i = 0; i < tricksPerRound && trickTakers.size() >= i * tricksPerRound + tricksPerRound; i++)
    {
        std::vector<Seat> roundTricks;
        for (int j = 0; j < tricksPerRound; j++)
        {
            roundTricks.push_back(trickTakers[j + i * tricksPerRound]);
        }
        roundTrickTakers.push_back(roundTricks);
    }
    return roundTrickTakers;
}

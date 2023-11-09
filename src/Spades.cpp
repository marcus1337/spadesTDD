#include "Spades.h"
#include <cassert>
#include "history/SpadesCommand.h"

using namespace spd;

SpadesMemento Spades::makeMemento() const
{
    auto memento = SpadesMemento();
    memento.seed = getSeed();
    memento.bidVariationType = (unsigned int)getBidVariationType();
    memento.trumpVariationType = (unsigned int)getTrumpVariationType();
    memento.bids = state.bids;
    memento.historyEncoding = history.serialize();
    memento.setPlayedSeatCardPairsData(state.playedSeatCardPairs);
    memento.setRoundBidOptionsData(state.roundBidOptions);
    memento.setTrickTakersData(state.trickTakers);
    return memento;
}

void Spades::loadMemento(const SpadesMemento &memento)
{
    reset(memento.seed, memento.getBidVariationType(), memento.getTrumpVariationType());
    state.bids = memento.bids;
    if (!history.deserialize(memento.historyEncoding))
    {
        std::cerr << "Failed deserializing history\n";
        reset();
    }
    state.playedSeatCardPairs = memento.getPlayedSeatCardPairs();
    state.roundBidOptions = memento.getRoundBidOptions();
    state.trickTakers = memento.getTrickTakers();
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
    state.deck.setExcludeCards(trumpVariationController.getExcludedCards());
}

void Spades::setSeed(unsigned int seed)
{
    assert(!state.hasGameStarted());
    state.deck.setSeed(seed);
}

Spades::Spades()
{
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
    return makeMemento().serialize();
}

void Spades::deserialize(const std::string &data)
{
    loadMemento(SpadesMemento(data));
}

Score Spades::getScore() const
{
    return Score{};
}

unsigned int Spades::getSeed() const
{
    return state.deck.getSeed();
}

Seat Spades::getTurnSeat() const
{
    return turn.getTurnSeat(state);
}

void Spades::addBid(int bid)
{
    auto bidCommand = std::make_unique<BidCommand>(bid);
    bidCommand->execute(state, turn, trumpVariationController);
    history.addCommand(std::move(bidCommand));
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
    return bidVariationController.getBids(seat, state);
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
    return Spades::state.getHand(seat);
}

void Spades::setBidOption(const Seat &seat, const BidOption &bidOption)
{
    auto bidOptCommand = std::make_unique<BidOptionCommand>(bidOption, seat);
    bidOptCommand->execute(state, turn, trumpVariationController);
    history.addCommand(std::move(bidOptCommand));
}

std::optional<int> Spades::getBidResult(const Seat &seat) const
{
    return bidVariationController.getBidResult(seat, state);
}

void Spades::place(const Card &card)
{
    auto placeCommand = std::make_unique<PlaceCommand>(card);
    placeCommand->execute(state, turn, trumpVariationController);
    history.addCommand(std::move(placeCommand));
}

bool Spades::canPlace(const Card &card) const
{
    return trumpVariationController.canPlaceCard(state, card, turn.getTurnSeat(state));
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
    history.undo(state, turn, trumpVariationController);
}

bool Spades::canUndo() const
{
    return history.canUndo();
}

void Spades::redo()
{
    history.redo(state, turn, trumpVariationController);
}

bool Spades::canRedo() const
{
    return history.canRedo();
}

std::vector<std::pair<Seat, Card>> Spades::getPlayedTrickSeatCardPairs() const
{
    return state.getPlayedTrickCardSeatPairs();
}

bool Spades::isStateValid() const
{
    return false;
}

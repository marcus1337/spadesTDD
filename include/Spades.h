#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <memory>
#include "data/Score.h"
#include "table/Deck.h"
#include "rules/BidVariationController.h"
#include "rules/TrumpVariationController.h"
#include "data/memento/SpadesMemento.h"
#include <array>
#include <vector>
#include "data/State.h"
#include "rules/Turn.h"

namespace spd
{
    class Spades
    {
        State state;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;
        Turn turn;

        SpadesMemento makeMemento() const
        {
            auto memento = SpadesMemento();
            memento.seed = getSeed();
            memento.bidVariationType = (unsigned int)getBidVariationType();
            memento.trumpVariationType = (unsigned int)getTrumpVariationType();
            memento.bids = state.bids;
            return memento;
        }

        void loadMemento(const SpadesMemento &memento)
        {
            reset(memento.seed, memento.getBidVariationType(), memento.getTrumpVariationType());
            state.bids = memento.bids;
        }

        void setBidVariation(BidVariationType type)
        {
            assert(!state.hasGameStarted());
            bidVariationController.setBidVariationType(type);
        }

        void setTrumpVariation(TrumpVariationType type)
        {
            assert(!state.hasGameStarted());
            trumpVariationController.setTrumpVariationType(type);
        }

        void setSeed(unsigned int seed)
        {
            assert(!state.hasGameStarted());
            state.setSeed(seed);
        }

    public:
        Spades()
        {
        }

        void reset(int seed, BidVariationType bidVariationType, TrumpVariationType trumpVariationType)
        {
            state.clear();
            setSeed(seed);
            setBidVariation(bidVariationType);
            setTrumpVariation(trumpVariationType);
        }

        void reset(BidVariationType bidVariationType, TrumpVariationType trumpVariationType)
        {
            reset(getSeed(), bidVariationType, trumpVariationType);
        }

        void reset(BidVariationType bidVariationType)
        {
            reset(getSeed(), bidVariationType, getTrumpVariationType());
        }

        void reset(TrumpVariationType trumpVariationType)
        {
            reset(getSeed(), getBidVariationType(), trumpVariationType);
        }

        void reset(unsigned int seed){
            reset(seed, getBidVariationType(), getTrumpVariationType());
        }

        void reset(){
            reset(getSeed());
        }

        void resetAndRandomizeSeed(){
            reset(PortableRandom::makeRandomSeed());
        }

        BidVariationType getBidVariationType() const
        {
            return bidVariationController.getBidVariationType();
        }

        TrumpVariationType getTrumpVariationType() const
        {
            return trumpVariationController.getTrumpVariationType();
        }

        std::string serialize() const
        {
            return makeMemento().serialize();
        }

        void deserialize(const std::string &data)
        {
            loadMemento(SpadesMemento(data));
        }

        Score getScore() const
        {
            return Score{};
        }

        unsigned int getSeed() const
        {
            return state.getSeed();
        }

        Seat getTurnSeat() const
        {
            return turn.getTurnSeat(state);
        }

        void addBid(int bid)
        {
            state.bids.push_back(bid);
        }

        bool hasBid(const Seat &seat) const
        {
            return state.hasBid(seat);
        }

        bool isBidPhase() const
        {
            return state.isBidPhase();
        }

        std::vector<int> getPossibleBids(const Seat &seat) const
        {
            return bidVariationController.getBids(seat, state);
        }

        std::vector<int> getPossibleBids() const {
            return getPossibleBids(getTurnSeat());
        }

        std::vector<BidOption> getBidOptions(const Seat &seat) const
        {
            return bidVariationController.getBidOptions(seat, state);
        }

        std::vector<Card> getHand(const Seat& seat) const {
            return state.getHand(seat);
        }

        void setBidOption(const Seat &seat, const BidOption &bidOption)
        {
            state.setBidOption(seat, bidOption);
        }

        std::optional<int> getBidResult(const Seat& seat) const {
            return bidVariationController.getBidResult(seat, state);
        }

        void playCard(const Card& card){
            state.playCard(getTurnSeat(), card);
        }

    };
}
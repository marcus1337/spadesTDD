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

namespace spd
{
    class Spades
    {
        Deck deck;
        State state;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;

        SpadesMemento makeMemento() const
        {
            auto memento = SpadesMemento();
            memento.seed = getSeed();
            memento.bidVariation = (int)bidVariationController.getBidVariationType();
            memento.trumpVariation = (int)trumpVariationController.getTrumpVariationType();
            memento.bids = state.bids;
            return memento;
        }

        void loadMemento(const SpadesMemento &memento)
        {
            setSeed(memento.seed);
            if (memento.bidVariation < (unsigned int)BidVariationType::LAST)
            {
                bidVariationController.setBidVariationType((BidVariationType)memento.bidVariation);
            }
            if (memento.trumpVariation < (unsigned int)BidVariationType::LAST)
            {
                trumpVariationController.setTrumpVariationType((TrumpVariationType)memento.bidVariation);
            }
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
            deck.setSeed(seed);
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
            reset(PortableRandom::makeRandomSeed(), bidVariationType, trumpVariationType);
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
            return deck.getSeed();
        }

        Seat getTurnSeat() const
        {
            return state.getTurnSeat();
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

        std::vector<BidOption> getBidOptions(const Seat &seat) const
        {
            return bidVariationController.getBidOptions(seat, state);
        }

        void setBidOption(const Seat &seat, const BidOption &bidOption)
        {
            state.setBidOption(seat, bidOption);
        }
    };
}
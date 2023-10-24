#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <memory>
#include "data/Score.h"
#include "table/Deck.h"
#include "rules/BidVariationController.h"
#include "rules/TrumpVariationController.h"
#include "data/SpadesMemento.h"
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
        std::array<Player, 4> players{};

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

    public:
        Spades()
        {
        }

        void reset()
        {
            state.clear();
        }

        void setBidVariation(BidVariationType type)
        {
            assert(!state.hasGameStarted());
            bidVariationController.setBidVariationType(type);
        }

        BidVariationType getBidVariationType() const
        {
            return bidVariationController.getBidVariationType();
        }

        void setTrumpVariation(TrumpVariationType type)
        {
            assert(!state.hasGameStarted());
            trumpVariationController.setTrumpVariationType(type);
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

        void setSeed(int seed)
        {
            assert(!state.hasGameStarted());
            deck.setSeed(seed);
        }

        int getSeed() const
        {
            return deck.getSeed();
        }

        const Player &getPlayer(const Seat &seat) const
        {
            return players[(int)seat];
        }

        bool hasPlayerBid(const Seat &seat) const
        {
            return state.hasPlayerBid(seat);
        }

        bool isBidPhase() const
        {
            return state.isBidPhase();
        }

        Seat getTurnSeat() const
        {
            return state.getTurnSeat();
        }

        void addBid(int bid)
        {
            state.bids.push_back(bid);
        }
    };
}
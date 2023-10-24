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

namespace spd
{
    class Spades
    {
        Deck deck;
        int round = 0;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;
        std::array<Player,4> players{};
        std::vector<int> bids;

        SpadesMemento makeMemento() const
        {
            auto memento = SpadesMemento();
            memento.round = round;
            memento.seed = getSeed();
            memento.bidVariation = (int)bidVariationController.getBidVariationType();
            memento.trumpVariation = (int)trumpVariationController.getTrumpVariationType();
            return memento;
        }

        void loadMemento(const SpadesMemento &memento)
        {
            round = memento.round;
            setSeed(memento.seed);
            if (memento.bidVariation < (unsigned int)BidVariationType::LAST)
            {
                bidVariationController.setBidVariationType((BidVariationType)memento.bidVariation);
            }
            if (memento.trumpVariation < (unsigned int)BidVariationType::LAST)
            {
                trumpVariationController.setTrumpVariationType((TrumpVariationType)memento.bidVariation);
            }
        }

    public:
        Spades()
        {
        }

        void startNewGame()
        {
            round = 1;
        }

        void endGame()
        {
            round = 0;
        }

        bool setBidVariation(BidVariationType type)
        {
            if (!hasStarted())
            {
                bidVariationController.setBidVariationType(type);
            }
            return !hasStarted();
        }

        BidVariationType getBidVariationType() const
        {
            return bidVariationController.getBidVariationType();
        }

        bool setTrumpVariation(TrumpVariationType type)
        {
            if (!hasStarted())
            {
                trumpVariationController.setTrumpVariationType(type);
            }
            return !hasStarted();
        }

        TrumpVariationType getTrumpVariationType() const
        {
            return trumpVariationController.getTrumpVariationType();
        }

        bool hasStarted() const
        {
            return round > 0;
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

        bool setSeed(int seed)
        {
            if (!hasStarted())
            {
                deck.setSeed(seed);
            }
            return !hasStarted();
        }

        int getSeed() const
        {
            return deck.getSeed();
        }

        const Player& getPlayer(const Seat& seat) const{
            return players[(int) seat];
        }

        bool hasPlayerBid(const Seat& seat) const {
            
            return false;
        }

        bool isBidPhase() const {
            return bids.size() % 4 != 0;
        }

        Seat getTurnSeat() const {
            return Seat::SOUTH;
        }

        void addBid(int bid){
            bids.push_back(bid);
        }

    };
}
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
        std::array<Player, 4> players{};
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

        int getRoundIndex() const
        {
            return round - 1;
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

        void setBidVariation(BidVariationType type)
        {
            assert(!hasStarted());
            bidVariationController.setBidVariationType(type);
        }

        BidVariationType getBidVariationType() const
        {
            return bidVariationController.getBidVariationType();
        }

        void setTrumpVariation(TrumpVariationType type)
        {
            assert(!hasStarted());
            trumpVariationController.setTrumpVariationType(type);
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

        void setSeed(int seed)
        {
            assert(!hasStarted());
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

        Seat getStartBidder(int round) const
        {
            return (Seat)((round - 1) % 4);
        }

        bool hasPlayerBid(const Seat &seat) const
        {
            if (!isBidPhase())
            {
                return false;
            }
            else
            {
                int madeBids = bids.size() % 4;
                Seat startBidder = getStartBidder(round);
                for (int i = 0; i < madeBids; i++)
                {
                    int seatIndex = ((int)startBidder + i) % 4;
                    if ((Seat)seatIndex == seat)
                    {
                        return true;
                    }
                }
                return false;
            }
        }

        bool isBidPhase() const
        {
            return bids.size() < round * 4;
        }

        Seat getTurnSeat() const
        {
            if (isBidPhase())
            {
                int playerIndex = (bids.size() + getRoundIndex()) % 4;
                return (Seat)playerIndex;
            }
            return Seat::SOUTH;
        }

        void addBid(int bid)
        {
            assert(hasStarted());
            bids.push_back(bid);
        }
    };
}
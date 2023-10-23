#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <memory>
#include "data/Score.h"
#include "Deck.h"
#include "rules/BidVariationController.h"
#include "rules/TrumpVariationController.h"
#include "data/SpadesMemento.h"

namespace spd
{
    class Spades
    {
        Deck deck;
        int round = 0;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;

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
    };
}
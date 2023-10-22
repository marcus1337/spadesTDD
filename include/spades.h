#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include "BidVariation.h"
#include <memory>

namespace spd
{
    struct CardEvaluator {
        virtual int getValue(const Card& card) = 0;
    };

    class Spades
    {
        bool started = false;
        std::unique_ptr<BidVariation> bidVariation;

    public:

        Spades() {
            setBidVariation(std::make_unique<DoubleBlindNil>());
        }

        void startNewGame() {
            started = true;
        }

        void endGame(){
            started = false;
        }

        bool setBidVariation(std::unique_ptr<BidVariation> bidVariation){
            if(!hasStarted()){
                this->bidVariation = std::move(bidVariation);
            }
            return !hasStarted();
        }

        BidVariation* getBidVariation() const {
            return bidVariation.get();
        }

        bool hasStarted() const {
            return started;
        }

        std::string serialize() const { //TODO: implement
            return "";
        }

        void deserialize(const std::string& data) { //TODO: implement
        }

    };
}
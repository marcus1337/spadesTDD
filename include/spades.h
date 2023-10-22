#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include "rules/BidVariation.h"
#include "rules/TrumpVariation.h"
#include <memory>
#include "data/Score.h"
#include "Deck.h"
#include "rules/BidVariationController.h"

namespace spd
{
    class Spades
    {
        Deck deck;
        bool started = false;
        BidVariationController bidVariationController;
        std::unique_ptr<TrumpVariation> trumpVariation;

    public:

        Spades() {

        }

        void startNewGame() {
            started = true;
        }

        void endGame(){
            started = false;
        }

        bool setBidVariation(BidVariationType type){
            if(!hasStarted()){
                bidVariationController.setType(type);
            }
            return !hasStarted();
        }

        BidVariationType getBidVariation() const {
            return bidVariationController.getBidVariationType();
        }

        bool setTrumpVariation(std::unique_ptr<TrumpVariation> trumpVariation){
            if(!hasStarted()){
                this->trumpVariation = std::move(trumpVariation);
            }
            return !hasStarted();
        }

        const TrumpVariation* getTrumpVariation() const {
            return trumpVariation.get();
        }

        bool hasStarted() const {
            return started;
        }

        std::string serialize() const { //TODO: implement
            return "";
        }

        void deserialize(const std::string& data) { //TODO: implement
        }

        Score getScore() const {
            return Score{};
        }

        bool setSeed(int seed){
            if(!hasStarted()){
                deck.setSeed(seed);
            }
            return !hasStarted();
        }

        int getSeed() const {
            return deck.getSeed();
        }

    };
}
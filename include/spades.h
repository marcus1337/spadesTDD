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
        bool started = false;
        BidVariationController bidVariationController;
        TrumpVariationController trumpVariationController;

        SpadesMemento makeMemento() const{
            return SpadesMemento{};
        }

        void loadMemento(const SpadesMemento& memento){

        }

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
                bidVariationController.setBidVariationType(type);
            }
            return !hasStarted();
        }

        BidVariationType getBidVariationType() const {
            return bidVariationController.getBidVariationType();
        }

        bool setTrumpVariation(TrumpVariationType type){
            if(!hasStarted()){
                trumpVariationController.setTrumpVariationType(type);
            }
            return !hasStarted();
        }

        TrumpVariationType getTrumpVariationType() const {
            return trumpVariationController.getTrumpVariationType();
        }

        bool hasStarted() const {
            return started;
        }

        std::string serialize() const {
            return makeMemento().serialize();
        }

        void deserialize(const std::string& data) {
            loadMemento(SpadesMemento(data));
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
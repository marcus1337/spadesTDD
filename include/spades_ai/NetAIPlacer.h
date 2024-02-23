#pragma once
#include "spades_ai/AIStrategy.h"
#include "spades_ai/AIPlacer.h"
#include "spades_ai/Serializable.h"
#include "spades_ai/neuralnet/Neuralnet.h"
#include <variant>

namespace spd
{
    class NetAIPlacer : public AIPlacer, public Serializable
    {
    public:
        NetAIPlacer() : network(5, 10, 8)
        {
        }
        virtual Card getPlacement(const Spades &spades) override
        {
            if (shouldAttack(spades))
                return getAttackPlacement(spades);
            else
                return getDefendPlacement(spades);
        }
        virtual std::string serialize() const override
        {
            return "";
        }
        virtual bool deserialize(const std::string &encoding) override
        {
            return true;
        }

    private:
        // bool in (4): spadesBroken, any opponent need tricks, need tricks, team player need tricks, 
        // bool in (3): any opponent protects nil, team player protects nil, protecting nil
        // in: startHand, currentHand, playedCards, other player might have suit (suits*num_other_players)
        // out (8): should place suit{TRUMP,D,H,C}? Card strength?(0..1) - {TRUMP,D,H,C}
        neuralnet::NeuralNet network;

        bool shouldAttack(const Spades &spades)
        {
            return true;
        }

        Card getAttackPlacement(const Spades &spades)
        {
            return Card();
        }
        Card getDefendPlacement(const Spades &spades)
        {
            return Card();
        }
    };
}
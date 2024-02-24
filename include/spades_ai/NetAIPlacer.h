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
        NetAIPlacer() : network(39, 47, 8)
        {
        }
        virtual Card getPlacement(const Spades &spades) override
        {
            if (tryWinLeadSuit(spades))
                return getWinPlacement(spades);
            else
                return getLosePlacement(spades);
        }
        virtual std::string serialize() const override
        {
            return "";
        }
        virtual bool deserialize(const std::string &encoding) override
        {
            return true;
        }

        void mutate()
        {
            network.randomizeWeightSubset();
        }

    private:
        // bool in  (4): spadesBroken, any opponent need tricks, need tricks, team player need tricks,
        // bool in  (3): any opponent protects nil, team player protects nil, protecting nil
        // bool in  (12): player might have suit (suits*num_other_players)
        // bool in  (4): has_topcard_in_hand(#suits)
        // bool in  (4): placed_lead_suit(#suits)
        // bool in  (4): #num_placed_trick_cards
        // float in (4): percentage_of_remaining_cards_in_hand(#suits), example: out of all remaining cards of type #suit - how large perc. in my hand? (special case when no remaining cards: input is 0)
        // float in (4): percentage_of_remaining_cards(#suits), example: out of all remaining cards how large percentage is of type #suit?

        // bool  out (4): place suit if possible {TRUMP,D,H,C}
        // bool  out (1): try win current trick?
        // float out (1): Suit card strength?(0..1), strength of the subset of cards that are either losing or winning current trick
        neuralnet::NeuralNet network;

        bool tryWinLeadSuit(const Spades &spades)
        {
            return true;
        }

        Card getWinPlacement(const Spades &spades)
        {
            return Card();
        }
        Card getLosePlacement(const Spades &spades)
        {
            return Card();
        }
    };
}
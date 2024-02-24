#pragma once
#include "spades_ai/AIStrategy.h"
#include "spades_ai/AIPlacer.h"
#include "spades_ai/Serializable.h"
#include "spades_ai/neuralnet/Neuralnet.h"
#include "spades_ai/Analyze.h"
#include <variant>

namespace spd
{
    class NetAIPlacer : public AIPlacer, public Serializable
    {
    public:
        NetAIPlacer() : network(39, 20, 9)
        {
        }
        virtual Card getPlacement(const Spades &spades) override
        {
            const auto output = network.getOutput(getObservation(spades));
            if (tryWinTrick(spades, output))
                return getWinPlacement(spades, output);
            else
                return getLosePlacement(spades, output);
        }
        virtual std::string serialize() const override
        {
            return network.encode();
        }
        virtual bool deserialize(const std::string &encoding) override
        {
            network = neuralnet::NeuralNet(encoding);
            return network.getNumLayers() == 3;
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
        // float out (4): Suit card strength?(0..1), strength of the subset of cards that are either losing or winning current trick
        // bool  out (1): try win current trick?
        neuralnet::NeuralNet network;

        std::vector<float> getObservation(const Spades &spades) const
        {
            return {};
        }

        bool tryWinTrick(const Spades &spades, const std::vector<float> &netOutput) const
        {
            return netOutput.back() >= 0.5f;
        }

        std::vector<Card> getPlaceableWinCards(const Spades &spades) const
        {
            std::vector<Card> winCards;
            for (const auto &card : spades.getPlaceableCards())
            {
                if (spades.isTopCardIfPlaced(card))
                {
                    winCards.push_back(card);
                }
            }
            return winCards;
        }
        std::vector<Card> getPlaceableLoseCards(const Spades &spades) const
        {
            std::vector<Card> loseCards;
            for (const auto &card : spades.getPlaceableCards())
            {
                if (!spades.isTopCardIfPlaced(card))
                {
                    loseCards.push_back(card);
                }
            }
            return loseCards;
        }

        std::vector<Card> getSuitCards(const Spades &spades, const Suit &suit, const std::vector<Card> &cards) const
        {
            std::vector<Card> suitCards;
            for (const auto &card : cards)
            {
                if ((spades.isTrumpCard(card) && suit == Suit::SPADE) || (!spades.isTrumpCard(card) && card.is(suit)))
                {
                    suitCards.push_back(card);
                }
            }
            return suitCards;
        }

        std::pair<Suit, std::vector<Card>> getPreferredSuitCards(const Spades &spades, const std::vector<float> &output, const std::vector<Card> &cards) const
        {
            for (const auto &suitIndex : getSuitIndicesDescending({output[0], output[1], output[2], output[3]}))
            {
                const auto suit = (Suit)suitIndex;
                const auto suitCards = getSuitCards(spades, suit, cards);
                if (!suitCards.empty())
                {
                    return {suit, suitCards};
                }
            }
            return {Suit::SPADE, {}};
        }

        std::vector<Card> getWinCards(const Spades &spades) const
        {
            auto cards = getPlaceableWinCards(spades);
            if (cards.empty())
                cards = getPlaceableLoseCards(spades);
            Analyze(spades).sortByStrengthAscending(cards);
            return cards;
        }
        std::vector<Card> getLoseCards(const Spades &spades) const
        {
            auto cards = getPlaceableLoseCards(spades);
            if (cards.empty())
                cards = getPlaceableWinCards(spades);
            Analyze(spades).sortByStrengthAscending(cards);
            return cards;
        }

        float getPreferredStrength(const Suit &suit, const std::vector<float> &output) const
        {
            return output.at(4 + (int)suit);
        }

        Card pickCard(const std::vector<Card> &cards, float strength) const
        {
            std::size_t index = std::roundf(cards.size() * strength);
            index = std::clamp<std::size_t>(index, 0, cards.size() - 1);
            return cards.at(index);
        }

        Card pickCard(const Spades &spades, const std::vector<float> &output, const std::vector<Card> &cards) const
        {
            auto suitCards = getPreferredSuitCards(spades, output, cards);
            return pickCard(suitCards.second, getPreferredStrength(suitCards.first, output));
        }

        Card getWinPlacement(const Spades &spades, const std::vector<float> &output) const
        {
            return pickCard(spades, output, getWinCards(spades));
        }
        Card getLosePlacement(const Spades &spades, const std::vector<float> &output) const
        {
            return pickCard(spades, output, getLoseCards(spades));
        }

        std::array<int, 4> getSuitIndicesDescending(const std::array<float, 4> &netOutput) const
        {
            std::array<std::pair<int, float>, 4> indices;
            for (int i = 0; i < 4; i++)
            {
                indices[i] = {i, netOutput[i]};
            }
            auto comparator = [](const std::pair<int, float> &a, const std::pair<int, float> &b)
            {
                return a.second > b.second;
            };
            std::sort(indices.begin(), indices.end(), comparator);
            return {indices[0].first, indices[1].first, indices[2].first, indices[3].first};
        }
    };
}
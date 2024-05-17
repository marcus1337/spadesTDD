#pragma once
#include "spades_ai/placer/AIPlacer.h"
#include "spades_ai/neuralnet/Neuralnet.h"
#include "spades_ai/logic/Analyze.h"
#include "spades_ai/logic/Observation.h"
#include "spades_ai/logic/Action.h"
#include <variant>
#include "spades/data/Serializable.h"
#include "spades_ai/neuralnet/WeightRandomizer.h"

namespace spd
{
    class NetAIPlacer : public AIPlacer, public Serializable
    {
    public:
        NetAIPlacer() : network(Observation::OBSERVATION_SIZE, (Observation::OBSERVATION_SIZE + Action::OUTPUT_SIZE) / 2, Action::OUTPUT_SIZE)
        {
        }
        virtual Card getPlacement(const Spades &spades) override
        {
            Observation observation(spades);
            const auto &observationValues = observation.getValues();
            const auto &outputValues = network.getOutput(std::vector<float>(observationValues.begin(), observationValues.end()));
            std::array<float, Action::OUTPUT_SIZE> arrOutputValues{};
            std::copy(outputValues.begin(), outputValues.end(), arrOutputValues.begin());
            Action action(spades, arrOutputValues);
            return action.getPlacement();
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
        neuralnet::NeuralNet network;
    };
}
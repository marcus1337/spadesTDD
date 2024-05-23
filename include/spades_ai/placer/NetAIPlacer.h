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
        NetAIPlacer();
        virtual Card getPlacement(const Spades &spades) const override;
        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &encoding) override;
        void mutate();

    private:
        neuralnet::NeuralNet network;
    };
}
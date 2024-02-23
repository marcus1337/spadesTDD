#pragma once
#include "spades_ai/AIPlacer.h"
#include "spades_ai/Serializable.h"
#include "spades_ai/neuralnet/Neuralnet.h"

namespace spd
{
    class NetAIPlacer : public AIPlacer, public Serializable
    {
    public:
        NetAIPlacer() = default;
        virtual Card getPlacement(const Spades &spades) override
        {
            return Card();
        }
        virtual std::string serialize() const override
        {
            return "";
        }
        virtual bool deserialize(const std::string &encoding) override
        {
            return true;
        }
    };
}
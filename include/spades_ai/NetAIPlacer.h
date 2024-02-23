#pragma once
#include "spades_ai/AIPlacer.h"

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
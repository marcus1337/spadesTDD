#pragma once
#include "spades_ai/Serializable.h"

namespace spd
{
    class Net :  public Serializable
    {
    public:
        Net() = default;
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
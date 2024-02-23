#pragma once
#include <string>

namespace spd
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;
        virtual std::string serialize() const = 0;
        virtual bool deserialize(const std::string &encoding) = 0;
    };
}

#pragma once
#include <string>

namespace spd
{

    class Serializable
    {
    public:
        virtual std::string serialize() const = 0;
        virtual bool deserialize(const std::string &encoding) = 0;
        virtual ~Serializable() = default;

    private:
    };

}
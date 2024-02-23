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
        NetAIPlacer() = default;
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
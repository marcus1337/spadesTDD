#pragma once
#include <memory>
#include <vector>
#include <string>
#include "spades/data/Move.h"

namespace spd
{
    class HistoryState : public Serializable
    {
    public:
        void clear();
        void add(const Move &item);
        const Move &peek() const;
        int getLeftSize() const;
        int getRightSize() const;
        void shiftRight();
        void shiftLeft();

        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &encoding) override;
    };

}
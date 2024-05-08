#pragma once
#include <memory>
#include <vector>
#include <string>
#include "spades/history/HistoryState.h"
#include "spades/data/State.h"
#include "spades/data/Move.h"

namespace spd
{
    class SpadesHistory : public Serializable
    {
        HistoryState historyState;
        void apply(State &state, const Move &move) const;
        void revert(State &state, const Move &move) const;

    public:
        SpadesHistory() = default;
        void undo(State &state);
        void redo(State &state);
        void clear();
        bool canUndo() const;
        bool canRedo() const;
        void addAndApply(State &state, const Move &move);

        virtual std::string serialize() const override;
        virtual bool deserialize(const std::string &encoding) override;
    };
}
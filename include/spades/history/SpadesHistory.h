#pragma once
#include <memory>
#include <vector>
#include <string>

namespace spd
{
    class SpadesHistory
    {

    public:

        void apply(State &state, const Move &move) const;
        void revert(State &state, const Move &move) const;
        void undo(State &state);
        void redo(State &state);
        void clear();
        bool canUndo() const;
        bool canRedo() const;
    };
}
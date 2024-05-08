#pragma once

namespace spd
{
    template <class T>
    class HistoryState
    {
    public:
        void clear();
        void add(const T &item);
        const T &peek() const;
        int getLeftSize() const;
        int getRightSize() const;
        void shiftRight();
        void shiftLeft();
    };

}
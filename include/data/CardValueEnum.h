#pragma once

namespace spd
{
    enum class Rank
    {
        ACE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        KNIGHT,
        QUEEN,
        KING
    };
    enum class Suit
    {
        SPADE = 0,
        HEART,
        DIAMOND,
        CLOVER
    };

    enum class Joker
    {
        LITTLE = 0,
        BIG
    };
}
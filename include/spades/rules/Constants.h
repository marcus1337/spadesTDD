#pragma once

namespace spd
{
    static constexpr unsigned int NUM_SEATS = 4;
    static constexpr unsigned int NUM_UNIQUE_CARDS = 54;
    static constexpr unsigned int DECK_SIZE = 52;
    static constexpr unsigned int HAND_SIZE = DECK_SIZE / NUM_SEATS;
    static constexpr unsigned int NUM_EXCLUDED_CARDS = NUM_UNIQUE_CARDS - DECK_SIZE;
}
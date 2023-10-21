#pragma once
#include <random>
#include <iostream>
#include <vector>

namespace spd {

    class PortableRandom {

        int seed;
        std::mt19937 mt;
        PortableRandom() : seed(std::random_device()()), mt(seed) {
        }

    public:

        static PortableRandom& getInstance() {
            static PortableRandom instance;
            return instance;
        }

        void setSeed(unsigned int newSeed) {
            seed = newSeed;
            mt = std::mt19937(seed);
        }

        unsigned int getSeed() const {
            return seed;
        }

        int randInt(int from, int to) { //TODO: Make or import portable uniform_distribution to be used with mt.
            int value = 0;
            return value;
        }

    };
}
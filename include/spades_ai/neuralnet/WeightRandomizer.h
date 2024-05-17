
#ifndef WEIGHTRANDOMIZER_H
#define WEIGHTRANDOMIZER_H

#include <vector>
#include <random>
#include <memory>
#include "spades_ai/neuralnet/Layer.h"

namespace neuralnet
{
    class WeightRandomizer
    {
        std::random_device randDevice;
        std::mt19937 rng;

        float getRandomWeight();
        std::size_t getRandomIndex(std::size_t numWeights);
        std::vector<std::size_t> getIndicesSubset(std::size_t numWeights);
        std::size_t getRandIndicesSubsetSize(std::size_t numWeights);

    public:
        WeightRandomizer();
        WeightRandomizer(WeightRandomizer &&other) noexcept;
        WeightRandomizer &operator=(WeightRandomizer &&other) noexcept;
        WeightRandomizer(const WeightRandomizer &) = delete;
        WeightRandomizer &operator=(const WeightRandomizer &) = delete;

        void randomize(std::vector<Layer> &layers);
        void randomize(Layer &layer);
        void randomizeSubset(Layer &layer);
        void randomizeSubset(std::vector<Layer> &layers);
    };
}

#endif

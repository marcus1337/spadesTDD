
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
        std::size_t getRandomWeightIndex(std::size_t numWeights);
        std::vector<std::size_t> getWeightIndicesSubset(std::size_t numWeights);
        std::size_t getWeightIndicesSubsetSize(std::size_t numWeights);

    public:
        WeightRandomizer();
        void randomize(std::vector<Layer>& layers);
        void randomize(Layer& layer);
        void randomizeSubset(Layer& layer);
    };
}

#endif

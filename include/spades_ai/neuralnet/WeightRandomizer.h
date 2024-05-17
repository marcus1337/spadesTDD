
#ifndef WEIGHTRANDOMIZER_H
#define WEIGHTRANDOMIZER_H

#include <vector>
#include <random>
#include <memory>

namespace neuralnet
{
    class WeightRandomizer
    {
        std::shared_ptr<std::mt19937> rng;
        std::vector<float> &weights;
        float getRandomWeight();
        int getRandomWeightIndex();
        std::vector<int> getWeightIndicesSubset();
        int getWeightIndicesSubsetSize();

    public:
        WeightRandomizer(std::vector<float> &_weights, std::shared_ptr<std::mt19937> rng);
        void radomize();
        void randomizeSubset();
    };
}

#endif


#ifndef WEIGHTRANDOMIZER_H
#define WEIGHTRANDOMIZER_H

#include <vector>
#include <random>

namespace neuralnet {
    class WeightRandomizer {
        std::vector<float>& weights;
        std::mt19937 rng;
        std::random_device randDevice;
        float getRandomWeight();
        int getRandomWeightIndex();
        std::vector<int> getWeightIndicesSubset();
        int getWeightIndicesSubsetSize();

    public:
        WeightRandomizer(std::vector<float>& _weights);
        void radomize();
        void randomizeSubset();

    };
}

#endif

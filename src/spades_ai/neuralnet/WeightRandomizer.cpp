
#include "spades_ai/neuralnet/WeightRandomizer.h"
#include <set>

namespace neuralnet {

    WeightRandomizer::WeightRandomizer(std::vector<float>& _weights) : weights(_weights) {
        rng.seed(randDevice());
    }

    void WeightRandomizer::radomize() {
        for (int i = 0; i < weights.size(); i++) {
            weights[i] = getRandomWeight();
        }
    }

    void WeightRandomizer::randomizeSubset() {
        for (int weightIndex : getWeightIndicesSubset()) {
            weights[weightIndex] = getRandomWeight();
        }
    }

    std::vector<int> WeightRandomizer::getWeightIndicesSubset() {
        std::set<int> indices;
        for (int i = 0; i < getWeightIndicesSubsetSize(); i++) {
            indices.insert(getRandomWeightIndex());
        }
        return std::vector<int>(indices.begin(), indices.end());
    }

    int WeightRandomizer::getWeightIndicesSubsetSize() {
        int maxSize = std::min<int>(3, weights.size());
        std::uniform_int_distribution<int> distribution(0, maxSize);
        return distribution(rng);
    }

    float WeightRandomizer::getRandomWeight() {
        std::uniform_real_distribution<float> distribution(-1.f, 1.f);
        return distribution(rng);
    }

    int WeightRandomizer::getRandomWeightIndex() {
        std::uniform_int_distribution<int> distribution(0, weights.size());
        return distribution(rng);
    }

}


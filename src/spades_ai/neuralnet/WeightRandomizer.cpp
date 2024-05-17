
#include "spades_ai/neuralnet/WeightRandomizer.h"
#include <set>

namespace neuralnet
{

    WeightRandomizer::WeightRandomizer() : rng(randDevice())
    {
    }

    void WeightRandomizer::randomize(Layer &layer)
    {
        for (std::size_t i = 0; i < layer.getNumInWeights(); i++)
        {
            layer.setInWeight(i, getRandomWeight());
        }
    }

    void WeightRandomizer::randomize(std::vector<Layer> &layers)
    {
        for (auto &layer : layers)
        {
            randomize(layer);
        }
    }

    void WeightRandomizer::randomizeSubset(Layer &layer)
    {
        for (std::size_t i : getWeightIndicesSubset(layer.getNumInWeights()))
        {
            layer.setInWeight(i, getRandomWeight());
        }
    }

    std::vector<std::size_t> WeightRandomizer::getWeightIndicesSubset(std::size_t numWeights)
    {
        std::set<std::size_t> indices;
        for (std::size_t i = 0; i < getWeightIndicesSubsetSize(numWeights); i++)
        {
            indices.insert(getRandomWeightIndex(numWeights));
        }
        return std::vector<std::size_t>(indices.begin(), indices.end());
    }

    std::size_t WeightRandomizer::getWeightIndicesSubsetSize(std::size_t numWeights)
    {
        std::size_t maxSize = std::max<std::size_t>(1, numWeights / 3);
        std::uniform_int_distribution<std::size_t> distribution(0, maxSize);
        return distribution(rng);
    }

    float WeightRandomizer::getRandomWeight()
    {
        std::uniform_real_distribution<float> distribution(-2.f, 2.f);
        return distribution(rng);
    }

    std::size_t WeightRandomizer::getRandomWeightIndex(std::size_t numWeights)
    {
        std::uniform_int_distribution<std::size_t> distribution(0, numWeights - 1);
        return distribution(rng);
    }

}

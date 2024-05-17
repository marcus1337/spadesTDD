
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
        for (std::size_t i : getIndicesSubset(layer.getNumInWeights()))
        {
            layer.setInWeight(i, getRandomWeight());
        }
    }

    void WeightRandomizer::randomizeSubset(std::vector<Layer> &layers)
    {
        for (std::size_t i : getIndicesSubset(layers.size()))
        {
            randomizeSubset(layers[i]);
        }
    }

    std::vector<std::size_t> WeightRandomizer::getIndicesSubset(std::size_t numIndices)
    {
        if (numIndices == 0)
        {
            return {};
        }
        std::set<std::size_t> indices;
        for (std::size_t i = 0; i < getRandIndicesSubsetSize(numIndices); i++)
        {
            indices.insert(getRandomIndex(numIndices));
        }
        return std::vector<std::size_t>(indices.begin(), indices.end());
    }

    std::size_t WeightRandomizer::getRandIndicesSubsetSize(std::size_t numIndices)
    {
        std::uniform_int_distribution<std::size_t> distribution(1, numIndices);
        return distribution(rng);
    }

    float WeightRandomizer::getRandomWeight()
    {
        std::uniform_real_distribution<float> distribution(-2.f, 2.f);
        return distribution(rng);
    }

    std::size_t WeightRandomizer::getRandomIndex(std::size_t size)
    {
        std::uniform_int_distribution<std::size_t> distribution(0, size - 1);
        return distribution(rng);
    }

}

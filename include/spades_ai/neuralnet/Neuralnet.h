
#ifndef NEURALNET_H
#define NEURALNET_H

#include <string>
#include <vector>
#include "Layer.h"
#include "WeightRandomizer.h"
#include "NetworkCodec.h"

namespace neuralnet
{

    class NeuralNet
    {
        std::vector<Layer> layers;
        WeightRandomizer randomizer;

    public:
        NeuralNet(int numInputNodes, int numHiddenNodes, int numOutputNodes);
        NeuralNet(const std::string &serializedLayers);
        NeuralNet(const NeuralNet &other);
        NeuralNet &operator=(const NeuralNet &other);

        std::string encode() const;
        static int selectMaxOutputIndex(std::vector<float> outputValues);
        std::vector<float> getOutput(std::vector<float> inputValues);
        bool isValidInput(const std::vector<float> &inputValues) const;
        void randomizeWeightSubset();
        std::size_t getNumLayers() const;
        std::vector<float> getInWeights(std::size_t layerIndex);
    };

}

#endif

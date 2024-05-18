
#include "spades_ai/neuralnet/Neuralnet.h"
#include <algorithm>

namespace neuralnet
{

    NeuralNet::NeuralNet(int numInputNodes, int numHiddenNodes, int numOutputNodes)
    {
        addLayers(numInputNodes, numHiddenNodes, numOutputNodes);
        randomize();
    }

    NeuralNet::NeuralNet(const std::string &serializedLayers)
    {
        layers = NetworkCodec::decode(serializedLayers);
    }

    NeuralNet::NeuralNet(const NeuralNet &other)
        : layers(other.layers), randomizer()
    {
    }

    void NeuralNet::addLayers(int numInputNodes, int numHiddenNodes, int numOutputNodes)
    {
        Layer inputLayer(numInputNodes, 0);
        layers.push_back(inputLayer);
        if (numHiddenNodes > 0)
        {
            Layer hiddenLayer(numHiddenNodes, layers[layers.size() - 1].getNumNodes());
            layers.push_back(hiddenLayer);
        }
        Layer outputLayer(numOutputNodes, layers[layers.size() - 1].getNumNodes());
        layers.push_back(outputLayer);
    }

    NeuralNet &NeuralNet::operator=(const NeuralNet &other)
    {
        if (this != &other)
        {
            layers = other.layers;
            randomizer = WeightRandomizer();
        }
        return *this;
    }

    std::string NeuralNet::encode() const
    {
        return NetworkCodec::encode(layers);
    }

    void NeuralNet::randomizeWeightSubset()
    {
        randomizer.randomizeSubset(layers);
    }

    void NeuralNet::randomize()
    {
        randomizer.randomize(layers);
    }

    int NeuralNet::selectMaxOutputIndex(std::vector<float> outputValues)
    {
        return std::distance(outputValues.begin(), std::max_element(outputValues.begin(), outputValues.end()));
    }

    bool NeuralNet::isValidInput(const std::vector<float> &inputValues) const
    {
        return inputValues.size() == layers.front().getNumNodes();
    }

    std::vector<float> NeuralNet::getOutput(std::vector<float> inputValues)
    {
        for (Layer &layer : layers)
        {
            inputValues = layer.getOutput(inputValues);
        }
        return inputValues;
    }

    std::size_t NeuralNet::getNumLayers() const
    {
        return layers.size();
    }

    std::vector<float> NeuralNet::getInWeights(std::size_t layerIndex)
    {
        return layers[layerIndex].getInWeights();
    }

}
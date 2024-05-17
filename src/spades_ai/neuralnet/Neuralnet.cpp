
#include "spades_ai/neuralnet/Neuralnet.h"
#include <algorithm>

namespace neuralnet
{

    NeuralNet::NeuralNet(int numInputNodes, int numHiddenNodes, int numOutputNodes)
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

    NeuralNet::NeuralNet(const std::string &serializedLayers)
    {
        layers = NetworkCodec::decode(serializedLayers);
    }

    std::string NeuralNet::encode() const
    {
        return NetworkCodec::encode(layers);
    }

    void NeuralNet::randomizeWeightSubset()
    {
        randomizer.randomizeSubset(layers);
    }

    Layer &NeuralNet::inputLayer()
    {
        return layers[0];
    }

    int NeuralNet::selectMaxOutputIndex(std::vector<float> outputValues)
    {
        return std::distance(outputValues.begin(), std::max_element(outputValues.begin(), outputValues.end()));
    }

    bool NeuralNet::isValidInput(const std::vector<float> &inputValues)
    {
        return inputValues.size() == inputLayer().getNumNodes();
    }

    std::vector<float> NeuralNet::getOutput(std::vector<float> inputValues)
    {
        for (Layer &layer : layers)
        {
            inputValues = layer.getOutput(inputValues);
        }
        return inputValues;
    }

    int NeuralNet::getNumLayers()
    {
        return layers.size();
    }

    std::vector<float> NeuralNet::getInWeights(int layerIndex)
    {
        return layers[layerIndex].getInWeights();
    }

}
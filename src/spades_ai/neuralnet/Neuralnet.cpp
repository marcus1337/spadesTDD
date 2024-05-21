
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
        int prevLayerNodes = numInputNodes;
        if (numHiddenNodes > 0)
        {
            Layer hiddenLayer(numHiddenNodes, prevLayerNodes);
            layers.push_back(hiddenLayer);
            prevLayerNodes = numHiddenNodes;
        }
        Layer outputLayer(numOutputNodes, prevLayerNodes);
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

    Eigen::VectorXf NeuralNet::getOutput(const Eigen::VectorXf &inputValues) const
    {
        Eigen::VectorXf currentInput = inputValues;
        for (const auto &layer : layers)
        {
            currentInput = layer.getOutput(currentInput);
        }
        return currentInput;
    }

    std::vector<float> NeuralNet::getOutput(const std::vector<float> &inputValues) const
    {
        Eigen::Map<const Eigen::VectorXf> eigenInput(inputValues.data(), inputValues.size());
        Eigen::VectorXf output = getOutput(eigenInput);
        return std::vector<float>(output.data(), output.data() + output.size());
    }

    std::size_t NeuralNet::getNumLayers() const
    {
        return layers.size() + 1; //+1 for input layer
    }

    std::vector<float> NeuralNet::getInWeights(std::size_t layerIndex) const
    {
        return layers[layerIndex].getInWeightsAsVec();
    }

}
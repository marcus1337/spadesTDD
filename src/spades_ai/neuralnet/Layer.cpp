#include "spades_ai/neuralnet/Layer.h"
#include <cmath>
#include "spades_ai/neuralnet/WeightRandomizer.h"

namespace neuralnet
{

    Layer::Layer(int layerSize, int previousLayerSize)
    {
        nodes.resize(layerSize);
        inWeights.resize(layerSize * previousLayerSize);
    }

    bool Layer::isInputLayer()
    {
        return inWeights.empty();
    }

    std::vector<float> Layer::getInWeights(int nodeIndex)
    {
        int sliceSize = getPreviousLayerSize();
        int startIndex = nodeIndex * sliceSize;
        int endIndex = startIndex + sliceSize;
        return std::vector<float>(inWeights.begin() + startIndex, inWeights.begin() + endIndex);
    }

    std::vector<float> Layer::getOutput(const std::vector<float> &inputValues)
    {
        if (isInputLayer())
            return std::vector<float>(inputValues.begin(), inputValues.end());

        std::vector<float> output;
        for (int i = 0; i < nodes.size(); i++)
        {
            Node &node = nodes[i];
            std::vector<float> nodeInWeights = getInWeights(i);
            node.setValue(nodeInWeights, inputValues);
            output.push_back(node.getValue());
        }
        return output;
    }

    int Layer::getNumNodes() const
    {
        return nodes.size();
    }

    std::vector<float> Layer::getInWeights() const
    {
        return inWeights;
    }

    void Layer::setInWeight(int index, float value)
    {
        inWeights[index] = value;
    }

    int Layer::getPreviousLayerSize()
    {
        return inWeights.size() / getNumNodes();
    }

}
#include "spades_ai/neuralnet/Layer.h"
#include <cmath>

namespace neuralnet
{

    Layer::Layer(std::size_t layerSize, std::size_t previousLayerSize) : inWeights(Eigen::MatrixXf::Random(layerSize, previousLayerSize))
    {
        assert(layerSize > 0);
        assert(previousLayerSize > 0);
    }

    Eigen::VectorXf Layer::sigmoid(const Eigen::VectorXf &x) const
    {
        return 1.0 / (1.0 + (-x.array()).exp());
    }

    std::vector<float> Layer::getInWeightsAsVec(std::size_t nodeIndex) const
    {
        const int sliceSize = getPreviousLayerSize();
        std::vector<float> result(sliceSize);
        for (int i = 0; i < sliceSize; ++i)
        {
            result[i] = inWeights(nodeIndex, i);
        }
        return result;
    }

    std::vector<float> Layer::getInWeightsAsVec() const
    {
        std::vector<float> flattenedWeights(inWeights.data(), inWeights.data() + inWeights.size());
        return flattenedWeights;
    }

    const Eigen::MatrixXf &Layer::getInWeights() const
    {
        return inWeights;
    }

    Eigen::VectorXf Layer::getOutput(const Eigen::VectorXf &inputValues) const
    {
        Eigen::VectorXf z = inWeights * inputValues;
        return sigmoid(z);
    }

    std::size_t Layer::getNumNodes() const
    {
        return inWeights.rows();
    }

    void Layer::setInWeight(std::size_t index, float value)
    {
        std::size_t row = index / inWeights.cols();
        std::size_t col = index % inWeights.cols();
        inWeights(row, col) = value;
    }

    std::size_t Layer::getPreviousLayerSize() const
    {
        return inWeights.cols();
    }
    std::size_t Layer::getNumInWeights() const
    {
        return inWeights.size();
    }

}

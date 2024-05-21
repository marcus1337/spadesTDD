
#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <vector>
#include <Eigen/Dense>

namespace neuralnet
{
    class Layer
    {
        Eigen::MatrixXf inWeights;
        std::size_t getPreviousLayerSize() const;

    public:
        Layer(std::size_t layerSize, std::size_t previousLayerSize);
        Layer(const Eigen::MatrixXf& inWeights);
        Eigen::VectorXf getOutput(const Eigen::VectorXf &inputValues) const;
        std::size_t getNumNodes() const;
        const Eigen::MatrixXf &getInWeights() const;
        std::vector<float> getInWeightsAsVec() const;
        std::vector<float> getInWeightsAsVec(std::size_t nodeIndex) const;
        std::size_t getNumInWeights() const;
        void setInWeight(std::size_t index, float value);
        Eigen::VectorXf sigmoid(const Eigen::VectorXf &x) const;

        std::size_t getNumRows() const;
        std::size_t getNumCols() const;
    };
}

#endif

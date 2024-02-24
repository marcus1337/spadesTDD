
#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <vector>
#include "Node.h"

namespace neuralnet {
    class Layer {
        std::vector<Node> nodes;
        std::vector<float> inWeights;
        std::vector<float> getInWeights(int nodeIndex);
        bool isInputLayer();
        int getPreviousLayerSize();
    public:
        Layer(int layerSize, int previousLayerSize);
        std::vector<float> getOutput(const std::vector<float>& inputValues);
        int getNumNodes() const;
        void randomizeWeightSubset();
        std::vector<float> getInWeights() const;
        void setInWeight(int index, float value);
    };
}

#endif

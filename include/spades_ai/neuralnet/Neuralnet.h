
#ifndef NEURALNET_H
#define NEURALNET_H

#include <string>
#include <vector>
#include "Layer.h"
#include "WeightRandomizer.h"
#include "NetworkCodec.h"

namespace neuralnet {

    class NeuralNet {
        std::vector<Layer> layers;
        Layer& inputLayer();
    public:
        NeuralNet(int numInputNodes, int numHiddenNodes, int numOutputNodes);
        NeuralNet(const std::string& serializedLayers);
        std::string encode();
        static int selectMaxOutputIndex(std::vector<float> outputValues);
        std::vector<float> getOutput(std::vector<float> inputValues);
        bool isValidInput(const std::vector<float>& inputValues);
        void randomizeWeightSubset();
        int getNumLayers();
        std::vector<float> getInWeights(int layerIndex);
    };

}

#endif

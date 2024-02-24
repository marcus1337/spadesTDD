
#include "spades_ai/neuralnet/NetworkCodec.h"
#include <sstream>

namespace neuralnet {

    std::string NetworkCodec::encode(const std::vector<Layer>& layers) {
        std::stringstream ss;
        for (const Layer& layer : layers) {
            ss << layer.getNumNodes() << " ";
            for (float weight : layer.getInWeights()) {
                ss << weight << " ";
            }
        }
        return ss.str();
    }

    std::vector<Layer> NetworkCodec::decode(const std::string& serializedLayers) {
        std::vector<Layer> layers;
        std::stringstream ss(serializedLayers);
        
        int layerSize;
        int previousLayerSize = 0;
        while (ss >> layerSize) {
            Layer layer(layerSize, previousLayerSize);
            int numInWeights = layerSize * previousLayerSize;
            for (int i = 0; i < numInWeights; i++) {
                float weight;
                ss >> weight;
                layer.setInWeight(i, weight);
            }
            previousLayerSize = layerSize;
            layers.push_back(layer);
        }
        return layers;
    }

}
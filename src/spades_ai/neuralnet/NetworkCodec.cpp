
#include "spades_ai/neuralnet/NetworkCodec.h"
#include <sstream>
#include <locale>

namespace neuralnet
{

    const std::string NetworkCodec::LOCALE_C = "C";

    std::string NetworkCodec::encode(const std::vector<Layer> &layers)
    {
        std::stringstream ss;
        ss.imbue(std::locale(LOCALE_C));
        for (const Layer &layer : layers)
        {
            ss << layer.getNumNodes() << " ";
            for (float weight : layer.getInWeightsAsVec()) {
                ss << weight << " ";
            }
        }
        return ss.str();
    }

    std::vector<Layer> NetworkCodec::decode(const std::string &serializedLayers)
    {
        std::vector<Layer> layers;
        std::stringstream ss(serializedLayers);
        ss.imbue(std::locale(LOCALE_C));

        int layerSize;
        int previousLayerSize = 0;
        while (ss >> layerSize)
        {
            Layer layer(layerSize, previousLayerSize);
            int numInWeights = layerSize * previousLayerSize;
            for (int i = 0; i < numInWeights; i++)
            {
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
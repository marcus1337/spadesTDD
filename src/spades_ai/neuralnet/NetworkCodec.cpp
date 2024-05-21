
#include "spades_ai/neuralnet/NetworkCodec.h"
#include <sstream>
#include <locale>

namespace neuralnet
{

    const std::string NetworkCodec::LOCALE_C = "C";
    const char NetworkCodec::SPACE = ' ';
    const char NetworkCodec::NL = '\n';

    std::string NetworkCodec::encode(const std::vector<Layer> &layers)
    {
        std::stringstream ss;
        ss.imbue(std::locale(LOCALE_C));

        ss << layers.size();
        ss << NL;
        for (const Layer &layer : layers)
        {
            ss << layer.getNumRows();
            ss << SPACE;
            ss << layer.getNumCols();
            ss << SPACE;
            ss << layer.getInWeights();
            ss << NL;
        }
        return ss.str();
    }

    std::vector<Layer> NetworkCodec::decode(const std::string &serializedLayers)
    {
        std::vector<Layer> layers;
        std::stringstream ss(serializedLayers);
        ss.imbue(std::locale(LOCALE_C));

        std::string token;
        std::size_t numLayers = 0;
        ss >> numLayers;
        for (std::size_t layerIndex = 0; layerIndex < numLayers; layerIndex++)
        {
            std::size_t numRows, numCols;
            ss >> numRows >> numCols;
            Eigen::MatrixXf matrix(numRows, numCols);
            for (int i = 0; i < numRows; ++i)
            {
                for (int j = 0; j < numCols; ++j)
                {
                    ss >> matrix(i, j);
                }
            }
            layers.push_back(Layer(matrix));
        }

        return layers;
    }

}
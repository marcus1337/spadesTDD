
#include "spades_ai/neuralnet/NetworkCodec.h"
#include <sstream>
#include <locale>

namespace neuralnet
{

    const std::string NetworkCodec::LOCALE_C = "C";
    const char NetworkCodec::DELIMITER = '\n';

    std::string NetworkCodec::encode(const std::vector<Layer> &layers)
    {
        std::stringstream ss;
        ss.imbue(std::locale(LOCALE_C));

        ss << layers.size();
        ss << DELIMITER;
        for (const Layer &layer : layers)
        {
            ss << layer.getNumRows();
            ss << DELIMITER;
            ss << layer.getNumCols();
            ss << DELIMITER;
            ss << layer.getInWeights();
            ss << DELIMITER;
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
            std::getline(ss, token, DELIMITER);
            int numRows = std::stoi(token);
            std::getline(ss, token, DELIMITER);
            int numCols = std::stoi(token);
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
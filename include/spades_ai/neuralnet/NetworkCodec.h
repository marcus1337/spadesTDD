
#ifndef NETWORKCODEC_H
#define NETWORKCODEC_H

#include "Layer.h"
#include <string>

namespace neuralnet
{
    class NetworkCodec
    {
    public:
        static std::string encode(const std::vector<Layer> &layers);
        static std::vector<Layer> decode(const std::string &serializedLayers);

    private:
        static const std::string LOCALE_C;
    };
}

#endif // !NETWORKCODEC_H

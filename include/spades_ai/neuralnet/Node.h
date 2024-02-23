
#ifndef NODE_H
#define NODE_H

#include <vector>


namespace neuralnet {

    class Node {
        float value = 0;
        float getSigmoid(float x);
        float getDotProduct(const std::vector<float>& weights, const std::vector<float>& inputValues);
        void activate();
    public:
        float getValue();
        void setValue(const std::vector<float>& weights, const std::vector<float>& inputValues);
    };

}

#endif

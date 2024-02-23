#include "spades_ai/neuralnet/Node.h"
#include <cmath>
#include <algorithm>
#include <iterator>

namespace neuralnet {

    float Node::getSigmoid(float x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    float Node::getDotProduct(const std::vector<float>& weights, const std::vector<float>& inputValues) {
        float dotProduct = 0.f;
        for (int i = 0; i < weights.size(); i++) {
            dotProduct += weights[i] * inputValues[i];
        }
        return dotProduct;
    }

    void Node::activate() {
        value = getSigmoid(value);
    }

    float Node::getValue() {
        return value;
    }

    void Node::setValue(const std::vector<float>& weights, const std::vector<float>& inputValues) {
        value = getDotProduct(weights, inputValues);
        activate();
    }

}
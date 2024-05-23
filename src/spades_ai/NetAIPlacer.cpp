#include "spades_ai/placer/NetAIPlacer.h"

using namespace spd;

NetAIPlacer::NetAIPlacer() : network(Observation::OBSERVATION_SIZE, (Observation::OBSERVATION_SIZE + Action::OUTPUT_SIZE) / 2, Action::OUTPUT_SIZE)
{
}
Card NetAIPlacer::getPlacement(const Spades &spades) const
{
    Observation observation(spades);
    const auto &observationValues = observation.getValues();
    const auto &outputValues = network.getOutput(std::vector<float>(observationValues.begin(), observationValues.end()));
    std::array<float, Action::OUTPUT_SIZE> arrOutputValues{};
    std::copy(outputValues.begin(), outputValues.end(), arrOutputValues.begin());
    Action action(spades, arrOutputValues);
    return action.getPlacement();
}
std::string NetAIPlacer::serialize() const
{
    return network.encode();
}
bool NetAIPlacer::deserialize(const std::string &encoding)
{
    network = neuralnet::NeuralNet(encoding);
    const auto numHiddenNodes = ((Observation::OBSERVATION_SIZE + Action::OUTPUT_SIZE) / 2);
    const auto layer1InWeights = numHiddenNodes * Observation::OBSERVATION_SIZE;
    const auto layer2InWeights = numHiddenNodes * Action::OUTPUT_SIZE;
    const auto &testInWeights1 = network.getInWeights(0);
    const auto &testInWeights2 = network.getInWeights(1);
    return network.getNumLayers() == 3 && testInWeights1.size() == layer1InWeights && testInWeights2.size() == layer2InWeights;
}

void NetAIPlacer::mutate()
{
    network.randomizeWeightSubset();
}

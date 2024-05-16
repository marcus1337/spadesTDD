#include "spades_ai/logic/NetAction.h"
#include "spades_ai/logic/NetUtils.h"

using namespace spd;
using namespace net;

NetAction::NetAction(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output)
{
    std::set<Card> handSet;
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        handSet.insert(card);
    }
    for (const auto &index : getIndicesSortedByMax(output))
    {
        const auto suitIndex = index / 4;
        const auto rankIndex = index % 13;
    }
}
Card NetAction::getPlacement() const
{
    return card;
}

std::vector<unsigned int> NetAction::getIndicesSortedByMax(const std::array<float, OUTPUT_SIZE> &output)
{
    std::vector<std::pair<float, unsigned int>> indexedValues;
    for (unsigned int i = 0; i < OUTPUT_SIZE; ++i)
    {
        indexedValues.emplace_back(output[i], i);
    }
    std::sort(indexedValues.begin(), indexedValues.end(), std::greater<>());
    std::vector<unsigned int> sortedIndices;
    for (const auto &pair : indexedValues)
    {
        sortedIndices.push_back(pair.second);
    }
    return sortedIndices;
}

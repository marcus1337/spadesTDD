#include "spades_ai/logic/Action.h"
#include "spades_ai/logic/NetUtils.h"

using namespace spd;
using namespace net;

Action::Action(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output)
{
    card = selectCard(spades, output);
}

Card Action::selectCard(const Spades &spades, const std::array<float, OUTPUT_SIZE> &output) const
{
    const auto &allCards = spades.getStartCards();
    std::set<Card> placeableCards;
    for (const auto &card : spades.getPlaceableCards())
    {
        placeableCards.insert(card);
    }

    for (const auto &index : getIndicesSortedByMax(output))
    {
        const auto &indexCard = allCards[index];
        if (placeableCards.contains(indexCard))
        {
            return indexCard;
        }
    }
    assert(false);
    return Card(0);
}

Card Action::getPlacement() const
{
    return card;
}

std::vector<unsigned int> Action::getIndicesSortedByMax(const std::array<float, OUTPUT_SIZE> &output) const
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

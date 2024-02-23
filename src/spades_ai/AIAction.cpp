#include "spades_ai/AIAction.h"
#include <array>
#include <algorithm>

using namespace spd;

AIAction::AIAction(const Spades &spades) : spades(spades)
{
}

Card AIAction::getRandomCard()
{
    const auto placeableCards = spades.getPlaceableCards();
    assert(!placeableCards.empty());
    const auto index = portableRandom.randInt(0, placeableCards.size() - 1);
    return placeableCards[index];
}

Card AIAction::getCard(std::vector<float> netOutput) const
{
    assert(netOutput.size() == 8);
    assert(!spades.getPlaceableCards().empty());
    std::array<std::pair<int, float>, 4> suitIndices;
    for (int i = 0; i < 4; i++)
    {
        suitIndices[i] = std::make_pair(i, netOutput[i]);
    }
    auto comparator = [](const std::pair<int, float> &a, const std::pair<int, float> &b)
    {
        return a.second > b.second;
    };
    std::sort(suitIndices.begin(), suitIndices.end(), comparator);

    for (const auto &suitIndex : suitIndices)
    {
        const auto placeableCards = Analyze(spades).getPlaceableCardsAscending((Suit)suitIndex.first);
        const auto cardStrength = netOutput.at(suitIndex.first + 4);
        if (!placeableCards.empty())
        {
            std::size_t cardIndex = (std::size_t)std::roundf(cardStrength * (float)placeableCards.size());
            cardIndex = std::clamp<std::size_t>(cardIndex, 0, placeableCards.size() - 1);
            return placeableCards[cardIndex];
        }
    }

    return Card();
}

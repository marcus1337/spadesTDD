#include "rules/TrumpVariation.h"
#include <cassert>
#include <string>

using namespace spd;

AceHigh::AceHigh() = default;
bool AceHigh::isTrumpCard(const Card &card) const
{
    return card.is(Suit::SPADE);
}

int AceHigh::getTrumpValue(const Card &card) const
{
    return 0;
}

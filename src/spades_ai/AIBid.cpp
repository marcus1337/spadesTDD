#include "spades_ai/logic/AIBid.h"

using namespace spd;

AIBid::AIBid(const Spades &spades) : spades(spades), analyze(spades)
{
}

int AIBid::getClosestNonZeroBid(int targetBid) const
{
    const auto possibleBids = spades.getPossibleBids();
    int bid = possibleBids.front();
    for (const auto possibleBid : possibleBids)
    {
        if (bid == 0 && possibleBid != 0)
        {
            bid = possibleBid;
        }
        else if (abs(possibleBid - targetBid) < abs(bid - targetBid))
        {
            bid = possibleBid;
        }
    }
    return bid;
}

bool AIBid::hasWeakHand() const
{
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        if (isStrongStartCard(card))
        {
            return false;
        }
    }
    return true;
}

int AIBid::getNumStrongCards() const
{
    const auto &hand = spades.getHand(spades.getTurnSeat());
    return std::count_if(hand.begin(), hand.end(), [this](const auto &card)
                         { return isStrongStartCard(card); });
}

int AIBid::getNumStrongNonTrumpCards() const
{
    int numStrongCards = 0;
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        if (!spades.isTrumpCard(card) && cardHasStrongRank(card))
        {
            numStrongCards++;
        }
    }
    return numStrongCards;
}

std::set<Rank> AIBid::getStrongRanks() const
{
    return {Rank::ACE, Rank::KING, Rank::QUEEN, Rank::KNIGHT};
}

bool AIBid::cardHasStrongRank(const Card &card) const
{
    const auto strongRanks = getStrongRanks();
    return std::any_of(strongRanks.begin(), strongRanks.end(), [card](const Rank &rank)
                       { return card.is(rank); });
}

bool AIBid::cardIsOfRank(const Card &card, const std::set<Rank> &possibleRanks) const
{
    for (const auto &rank : possibleRanks)
    {
        if (card.is(rank))
        {
            return true;
        }
    }
    return false;
}

bool AIBid::isStrongStartCard(const Card &card) const
{
    const auto suit = spades.getEffectiveSuit(card);
    if (suit == Suit::SPADE)
    {
        return true;
    }
    if (cardIsOfRank(card, {Rank::ACE, Rank::KING, Rank::QUEEN, Rank::KNIGHT}))
    {
        return true;
    }
    return false;
}

int AIBid::getBid() const
{
    const auto& possibleBids = spades.getPossibleBids();
    assert(!possibleBids.empty());

    if (possibleBids.size() == 1 || hasWeakHand())
    {
        return possibleBids.front();
    }
    else if (possibleBids.size() == 2)
    {
        return possibleBids.back();
    }
    else
    {
        const int minTakes = analyze.getGuaranteedTrickTakes(spades.getTurnSeat());
        const int probableTakes = getNumStrongNonTrumpCards() / 2; // Arbitrary divisor value
        const int lowCommonPartnerBid = 2;
        const int highCommonPartnerBid = 4;
        const int probableTargetBid = std::clamp(probableTakes, lowCommonPartnerBid, highCommonPartnerBid);
        return getClosestNonZeroBid(minTakes + probableTargetBid);
    }
}

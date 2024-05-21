#include "spades_ai/logic/NetUtils.h"

using namespace spd;
using namespace spd::net;

bool spd::net::wasFirstSpadePlacedStartOfTrick(const Spades &spades)
{
    uint64_t counter = 0;
    for (const auto &pair : spades.getCurrentRoundCardSeatPairs())
    {
        counter++;
        if (spades.getEffectiveSuit(pair.second) == Suit::SPADE)
        {
            return counter % 4 == 0;
        }
    }
    return false;
}
bool spd::net::hasStartedTrickBreakingSpades(const Spades &spades, const Seat &seat)
{
    std::size_t counter = 0;
    for (const auto &pair : spades.getCurrentRoundCardSeatPairs())
    {
        counter++;
        if (spades.getEffectiveSuit(pair.second) == Suit::SPADE)
        {
            return counter % 4 == 0 && pair.first == seat;
        }
    }
    return false;
}

bool spd::net::hasSkippedLeadSuit(const Spades &spades, const Suit &suit, const Seat &seat)
{
    const auto &csPairs = spades.getCurrentRoundCardSeatPairs();
    const auto cardsPerTrick = 4;
    for (int i = 0; i < spades.getRound() + 1; i++)
    {
        const auto from = i * cardsPerTrick;
        if (from < csPairs.size() && net::hasSkippedLeadSuit(spades, csPairs, from, suit, seat))
        {
            return true;
        }
    }
    return false;
}
bool spd::net::hasSkippedLeadSuit(const Spades &spades, const std::vector<std::pair<Seat, Card>> &csPairs, int from, const Suit &suit, const Seat &seat)
{
    const Suit leadSuit = spades.getEffectiveSuit(csPairs[from].second);
    for (int i = 1; i <= 3 && i + from < csPairs.size(); i++)
    {
        const auto &otherSeat = csPairs[from + i].first;
        const auto &otherSuit = spades.getEffectiveSuit(csPairs[from + i].second);
        if (otherSeat == seat && otherSuit != leadSuit)
        {
            return true;
        }
    }
    return false;
}
bool spd::net::areAllSuitCardsPlaced(const Spades &spades, const Suit &suit)
{
    for (const auto &card : spades.getUnplacedRoundCards())
    {
        const auto &unplacedSuit = spades.getEffectiveSuit(card);
        if (unplacedSuit == suit)
        {
            return false;
        }
    }
    return true;
}
bool spd::net::hasActiveNilBid(const Spades &spades, const Seat &seat)
{
    const auto bid = spades.getBidResult(seat).value();
    const auto &takenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    return bid == 0 && takenTricks == 0;
}
Seat spd::net::getRelativeSeatPosition(const Spades &spades, const Seat &relSeat)
{
    const auto &seat = spades.getTurnSeat();
    if (SeatUtils::getLeftOpponentSeat(seat) == relSeat)
    {
        return Seat::WEST;
    }
    if (SeatUtils::getRightOpponentSeat(seat) == relSeat)
    {
        return Seat::EAST;
    }
    return Seat::NORTH;
}
unsigned int spd::net::getRelativeSeatIndex(const Spades &spades, const Seat &relSeat)
{
    const auto &pos = net::getRelativeSeatPosition(spades, relSeat);
    if (pos == Seat::EAST)
    {
        return 0;
    }
    else if (pos == Seat::NORTH)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
unsigned int spd::net::getCardIndex(const Spades &spades, const Card &card)
{
    return net::getSuitIndex(spades, card) * 13 + net::getRankIndex(spades, card);
}
unsigned int spd::net::getSuitIndex(const Spades &spades, const Card &card)
{
    return (unsigned int)spades.getEffectiveSuit(card);
}
unsigned int spd::net::getRankIndex(const Spades &spades, const Card &card)
{
    unsigned int rankIndex = 0;
    if (spades.getEffectiveSuit(card) == Suit::SPADE)
    {
        const auto &trumpCards = spades.getTrumpCardsDescending();
        auto it = std::find(trumpCards.begin(), trumpCards.end(), card);
        if (it != trumpCards.end())
        {
            rankIndex = static_cast<unsigned int>(std::distance(trumpCards.begin(), it));
        }
    }
    else if (card.getRank().has_value())
    {
        const auto &ranks = Card::getRanks();
        const auto rank = card.getRank().value();
        auto it = std::find(ranks.begin(), ranks.end(), rank);
        if (it != ranks.end())
        {
            rankIndex = static_cast<unsigned int>(std::distance(ranks.begin(), it));
        }
    }
    rankIndex = std::clamp<unsigned int>(rankIndex, 0, 12);
    return rankIndex;
}
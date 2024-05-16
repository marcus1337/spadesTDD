#include "spades_ai/logic/Observation.h"

using namespace spd;

Observation::Observation(const Spades &spades) : observation{}
{
    seat = spades.getTurnSeat();
    oppSeat1 = SeatUtils::getLeftOpponentSeat(seat);
    oppSeat2 = SeatUtils::getRightOpponentSeat(seat);
    teamSeat = SeatUtils::getTeamSeat(seat);
    // TODO
}

const std::array<float, Observation::OBSERVATION_SIZE> &Observation::getValues() const
{
    return observation;
}

bool Observation::hasStartedTrickBreakingSpades(const Spades &spades, const Seat &seat) const
{
    if (!spades.isSpadesBroken())
    {
        return false;
    }
    uint64_t counter = 0;
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

bool Observation::wasFirstSpadePlacedStartOfTrick(const Spades &spades) const
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

bool Observation::areAllSuitCardsPlaced(const Spades &spades, const Suit &suit) const
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

bool Observation::hasSkippedLeadSuit(const Spades &spades, const std::vector<std::pair<Seat, Card>> &csPairs, int from, const Suit &suit, const Seat &seat) const
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

bool Observation::hasSkippedLeadSuit(const Spades &spades, const Suit &suit, const Seat &seat) const
{
    const auto &csPairs = spades.getCurrentRoundCardSeatPairs();
    const auto cardsPerTrick = 4;
    for (int i = 0; i < spades.getRound() + 1; i++)
    {
        const auto from = i * cardsPerTrick;
        if (from < csPairs.size() && hasSkippedLeadSuit(spades, csPairs, from, suit, seat))
        {
            return true;
        }
    }
    return false;
}

bool Observation::hasActiveNilBid(const Spades &spades, const Seat &seat) const
{
    const auto &bid = spades.getBidResult(seat).value();
    const auto &takenTricks = spades.getNumberOfTakenTricksCurrentRound(seat);
    return bid == 0 && takenTricks == 0;
}

std::array<float, DECK_SIZE> Observation::getPlacedCards(const Spades &spades) const
{
    std::array<float, DECK_SIZE> values{};
    for (const auto &[_, card] : spades.getCurrentRoundCardSeatPairs())
    {
        values[getCardIndex(spades, card)] = 1.f;
    }
    return values;
}
std::array<float, DECK_SIZE> Observation::getStartCards(const Spades &spades) const
{
    std::array<float, DECK_SIZE> values{};
    for (const auto &card : spades.getRoundStartCards(spades.getTurnSeat()))
    {
        values[getCardIndex(spades, card)] = 1.f;
    }
    return values;
}
std::array<float, DECK_SIZE> Observation::getCurrentlyOwnedCards(const Spades &spades) const
{
    std::array<float, DECK_SIZE> values{};
    for (const auto &card : spades.getHand(spades.getTurnSeat()))
    {
        values[getCardIndex(spades, card)] = 1.f;
    }
    return values;
}
std::array<float, 4> Observation::getLeadCardSuit(const Spades &spades) const
{
    std::array<float, 4> values{};
    const auto &trick = spades.getCurrentTrickCardSeatPairs();
    if (!trick.empty())
    {
        values[getSuitIndex(spades, trick.front().second)] = 1.f;
    }
    return values;
}

bool Observation::isTopCardTrump(const Spades &spades) const
{
    for (const auto &[_, card] : spades.getCurrentTrickCardSeatPairs())
    {
        if (spades.getEffectiveSuit(card) == Suit::SPADE)
        {
            return true;
        }
    }
    return false;
}

std::array<float, 13> Observation::getTopCardRank(const Spades &spades) const
{
    std::array<float, 13> values{};
    const auto &topSeat = spades.getCurrentTrickTopSeat();
    if (topSeat.has_value())
    {
        for (const auto &[seat, card] : spades.getCurrentTrickCardSeatPairs())
        {
            if (seat == topSeat)
            {
                values[getRankIndex(spades, card)] = 1.f;
            }
        }
    }
    return values;
}

unsigned int Observation::getSuitIndex(const Spades &spades, const Card &card) const
{
    return (unsigned int)spades.getEffectiveSuit(card);
}

unsigned int Observation::getRankIndex(const Spades &spades, const Card &card) const // Max 12
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
        const auto &rank = card.getRank().value();
        auto it = std::find(ranks.begin(), ranks.end(), rank);
        if (it != ranks.end())
        {
            rankIndex = static_cast<unsigned int>(std::distance(ranks.begin(), it));
        }
    }
    rankIndex = std::clamp<unsigned int>(rankIndex, 0, 12);
    return rankIndex;
}

std::array<float, NUM_SEATS - 1> Observation::getTopCardSeat(const Spades &spades) const
{
    std::array<float, NUM_SEATS - 1> values{};
    const auto &topSeat = spades.getCurrentTrickTopSeat();
    if (topSeat.has_value())
    {
        values[getRelativeSeatIndex(spades, topSeat.value())] = 1.f;
    }
    return values;
}
std::array<float, NUM_SEATS - 1> Observation::getPlacedTrickCardSeats(const Spades &spades) const
{
    std::array<float, NUM_SEATS - 1> values{};
    for (const auto &[seat, _] : spades.getCurrentTrickCardSeatPairs())
    {
        values[getRelativeSeatIndex(spades, seat)] = 1.f;
    }
    return values;
}
std::array<float, 4 * (NUM_SEATS - 1)> Observation::getKnownAbsentSuits(const Spades &spades) const
{
    std::array<float, 4 * (NUM_SEATS - 1)> values{};
    for (const auto &otherSeat : SeatUtils::getOtherSeats(spades.getTurnSeat()))
    {
        const auto &seatIndex = getRelativeSeatIndex(spades, otherSeat);
        for (const auto &suit : Card::getSuits())
        {
            values[seatIndex * 4 + (unsigned int)suit] = areAllSuitCardsPlaced(spades, suit) || hasSkippedLeadSuit(spades, suit, otherSeat) ? 1.f : 0.f;
        }
        if (hasStartedTrickBreakingSpades(spades, otherSeat))
        {
            for (const auto &suit : {Suit::CLOVER, Suit::DIAMOND, Suit::HEART})
            {
                values[seatIndex * 4 + (unsigned int)suit] = 1.f;
            }
        }
    }
    return values;
}
std::array<float, NUM_SEATS> Observation::getActiveNilBidSeats(const Spades &spades) const
{
    std::array<float, NUM_SEATS> values{};
    values[3] = hasActiveNilBid(spades, spades.getTurnSeat()) ? 1.f : 0.f;
    for (const auto &otherSeat : SeatUtils::getOtherSeats(spades.getTurnSeat()))
    {
        const auto &seatIndex = getRelativeSeatIndex(spades, otherSeat);
        values[seatIndex] = hasActiveNilBid(spades, otherSeat) ? 1.f : 0.f;
    }
    return values;
}
std::array<float, 13> Observation::getNumNeededTricksOpponent(const Spades &spades) const
{
    std::array<float, 13> values{};
    const auto totalBid = spades.getBidResult(oppSeat1).value() + spades.getBidResult(oppSeat2).value();
    for (int i = 0; i < totalBid; i++)
    {
        values[i] = 1.f;
    }
    return values;
}
std::array<float, 13> Observation::getNumNeededTricksTeam(const Spades &spades) const
{
    std::array<float, 13> values{};
    const auto totalBid = spades.getBidResult(seat).value() + spades.getBidResult(teamSeat).value();
    for (int i = 0; i < totalBid; i++)
    {
        values[i] = 1.f;
    }
    return values;
}

unsigned int Observation::getCardIndex(const Spades &spades, const Card &card) const // MAX 52-1
{
    return getSuitIndex(spades, card) * 13 + getRankIndex(spades, card);
}

Seat Observation::getRelativeSeatPosition(const Spades &spades, const Seat &relSeat) const
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

unsigned int Observation::getRelativeSeatIndex(const Spades &spades, const Seat &relSeat) const
{
    const auto &pos = getRelativeSeatPosition(spades, relSeat);
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

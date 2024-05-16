#include "spades_ai/logic/Observation.h"

using namespace spd;

Observation::Observation(const Spades &spades) : observation{}
{
    seat = spades.getTurnSeat();
    oppSeat1 = SeatUtils::getLeftOpponentSeat(seat);
    oppSeat2 = SeatUtils::getRightOpponentSeat(seat);
    teamSeat = SeatUtils::getTeamSeat(seat);
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
    for (const auto &card : spades.getUnplacedRoundCards())
    {
    }
    return {}; // TODO
}
std::array<float, DECK_SIZE> Observation::getStartCards(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, DECK_SIZE> Observation::getCurrentlyOwnedCards(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, 4> Observation::getLeadCardSuit(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, 13> Observation::getLeadCardRank(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, NUM_SEATS - 1> Observation::getLeadCardSeat(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, NUM_SEATS - 1> Observation::getPlacedTrickCardSeats(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, 4 * (NUM_SEATS - 1)> Observation::getKnownAbsentSuits(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, NUM_SEATS> Observation::getActiveNilBidSeats(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, 13> Observation::getNumNeededTricksOpponent(const Spades &spades) const
{
    return {}; // TODO
}
std::array<float, 13> Observation::getNumNeededTricksTeam(const Spades &spades) const
{
    return {}; // TODO
}

unsigned int Observation::getCardIndex(const Spades &spades, const Card &card) const // MAX 52-1
{
    const auto &suit = spades.getEffectiveSuit(card);
    const unsigned int suitIndex = (unsigned int)suit;
    unsigned int rankIndex = 0;

    if (suit == Suit::SPADE)
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
    return suitIndex * 13 + rankIndex;
}
#include "spades_ai/logic/Observation.h"
#include "spades_ai/logic/NetUtils.h"

using namespace spd;
using namespace spd::net;

Observation::Observation(const Spades &spades) : observation{}
{
    seat = spades.getTurnSeat();
    oppSeat1 = SeatUtils::getLeftOpponentSeat(seat);
    oppSeat2 = SeatUtils::getRightOpponentSeat(seat);
    teamSeat = SeatUtils::getTeamSeat(seat);

    std::size_t counter = 0;
    for (const auto &value : getPlacedCards(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getStartCards(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getCurrentlyOwnedCards(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getLeadCardSuit(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getTopCardRank(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getTopCardSeat(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getPlacedTrickCardSeats(spades))
    {
        observation[counter++] = value;
    }
    const auto &suitSupplyObs = SuitSupplyObservation(spades);
    for (const auto &value : suitSupplyObs.getValues())
    {
        observation[counter++] = value;
    }
    for (const auto &value : getActiveNilBidSeats(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getNumNeededTricksOpponent(spades))
    {
        observation[counter++] = value;
    }
    for (const auto &value : getNumNeededTricksTeam(spades))
    {
        observation[counter++] = value;
    }

    assert(counter == observation.size() - 1);
    observation.back() = isTopCardTrump(spades) ? 1.f : 0;
}

const std::array<float, Observation::OBSERVATION_SIZE> &Observation::getValues() const
{
    return observation;
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

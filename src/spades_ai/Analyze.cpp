#include "spades_ai/Analyze.h"

using namespace spd;

Analyze::Analyze(const Spades &spades) : spades(spades)
{
}

std::vector<Card> Analyze::getPlayedRoundCards(const Seat &targetSeat) const
{
    std::vector<Card> cards;
    for (const auto &[seat, card] : spades.getCurrentRoundCardSeatPairs())
    {
        if (targetSeat == seat)
        {
            cards.push_back(card);
        }
    }
    return cards;
}

std::vector<Card> Analyze::getRemainingNonSeatRoundCards(const Seat &seat) const
{
    std::vector<Card> remainingOtherCards;
    for (const auto &otherSeat : SeatUtils::getOtherSeats(seat))
    {
        for (const auto &card : spades.getHand(otherSeat))
        {
            remainingOtherCards.push_back(card);
        }
    }
    return remainingOtherCards;
}

bool Analyze::isEffectiveSuitInOtherHand(const Seat &perspectiveSeat, const Suit &suit) const
{
    for (const auto &seat : SeatUtils::getOtherSeats(perspectiveSeat))
    {
        for (const auto &card : spades.getHand(seat))
        {
            if (spades.getEffectiveSuit(card) == suit)
            {
                return true;
            }
        }
    }
    return false;
}

bool Analyze::isEffectiveSuitInAnyHand(const Suit &suit) const
{
    for (const auto &seat : SeatUtils::getSeats())
    {
        for (const auto &card : spades.getHand(seat))
        {
            if (spades.getEffectiveSuit(card) == suit)
            {
                return true;
            }
        }
    }
    return false;
}

int Analyze::getTrumpIndexDescending(const Card &card) const
{
    int counter = 0;
    for (const auto &trump : spades.getTrumpCardsDescending())
    {
        if (card == trump)
        {
            return counter;
        }
        counter++;
    }
    return -1;
}

std::set<Suit> Analyze::getUnfollowedEffectiveLeadSuits(const Seat &targetSeat) const
{
    std::set<Suit> voidSuits;
    const auto cardSeatPairs = spades.getCurrentRoundCardSeatPairs();
    for (int i = 0; i < cardSeatPairs.size(); i += NUM_SEATS)
    {
        for (int j = i + 1; j < i + NUM_SEATS && j < cardSeatPairs.size(); j++)
        {
            const auto leadPair = cardSeatPairs[i];
            const auto leadSuit = spades.getEffectiveSuit(leadPair.second);
            const auto leadSeat = leadPair.first;

            const auto followPair = cardSeatPairs[j];
            const auto followSuit = spades.getEffectiveSuit(followPair.second);
            const auto followSeat = followPair.first;

            if (leadSuit != followSuit && followSeat == targetSeat)
            {
                voidSuits.insert(leadSuit);
            }
        }
    }
    return voidSuits;
}

int Analyze::getGuaranteedTrickTakes(const Seat &seat) const
{
    const auto trumps = spades.getTrumpCardsDescending();
    std::vector<bool> trumpChecks(trumps.size(), false);
    std::vector<bool> trumpRejects(trumps.size(), true);
    for (const auto &card : spades.getHand(seat))
    {
        const int trumpIndex = getTrumpIndexDescending(card);
        if (trumpIndex >= 0)
        {
            trumpChecks[trumpIndex] = true;
            trumpRejects[trumpIndex] = false;
        }
    }

    for (int i = 0; i < trumps.size(); i++)
    {
        for (int j = i + 1; j < trumps.size(); j++)
        {
            if (trumpChecks[i] && trumpRejects[j])
            {
                trumpChecks[i] = false;
                trumpRejects[j] = false;
                break;
            }
        }
    }

    int numTricks = 0;
    for (const bool winTrick : trumpChecks)
    {
        if (winTrick)
        {
            numTricks++;
        }
    }

    return numTricks;
}

std::vector<Card> Analyze::getPlaceableCards(const Suit &suit) const
{
    std::vector<Card> cards;
    if (suit == Suit::SPADE)
    {
        for (const auto &card : spades.getHand(spades.getTurnSeat()))
        {
            for (const auto &trumpCard : spades.getTrumpCardsDescending())
            {
                if (trumpCard == card)
                {
                    cards.push_back(trumpCard);
                }
            }
        }
    }
    else
    {
        for (const auto &card : spades.getHand(spades.getTurnSeat()))
        {
            if (card.is(suit))
            {
                cards.push_back(card);
            }
        }
    }
    return cards;
}

std::vector<Card> Analyze::getPlaceableCardsAscending(const Suit &suit) const
{
    auto cards = getPlaceableCards(suit);
    sortByStrengthAscending(cards);
    return cards;
}

void Analyze::sortByStrengthAscending(std::vector<Card> &cards) const
{
    auto compare = [&](const Card &a, const Card &b)
    {
        int trumpValueA = getTrumpValue(a);
        int trumpValueB = getTrumpValue(b);
        if (trumpValueA == 0 && trumpValueB == 0)
            return getRankValue(a) < getRankValue(b);
        return trumpValueA < trumpValueB;
    };
    std::sort(cards.begin(), cards.end(), compare);
}

int Analyze::getTrumpValue(const Card &card) const
{
    const auto trumps = spades.getTrumpCardsDescending();
    for (int i = 0; i < trumps.size(); i++)
    {
        if (card == trumps[i])
        {
            return trumps.size() - i;
        }
    }
    return 0;
}
int Analyze::getRankValue(const Card &card) const
{
    int value = spades.getRanksDescending().size();
    for (const auto &rank : spades.getRanksDescending())
    {
        if (card.is(rank))
            return value;
        else
            value--;
    }
    return 0;
}
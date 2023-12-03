#include "test_pch.h"
#include "spades/Spades.h"
#include "spades_ai/Analyze.h"

using namespace spd;

struct SeatOrder
{
    const Seat seat1;
    const Seat seat2;
    const Seat seat3;
    const Seat seat4;
    SeatOrder(const Seat &seat1) : seat1(seat1), seat2(SeatUtils::getNextSeat(seat1)), seat3(SeatUtils::getNextSeat(seat2)), seat4(SeatUtils::getNextSeat(seat3))
    {
    }

    std::array<Seat, NUM_SEATS> getSeats() const
    {
        return {seat1, seat2, seat3, seat4};
    }

    std::array<Seat, NUM_SEATS - 1> getOtherSeats() const
    {
        return {seat2, seat3, seat4};
    }

    static std::array<SeatOrder, NUM_SEATS> getSeatOrders()
    {
        return {SeatOrder(Seat::SOUTH), SeatOrder(Seat::WEST), SeatOrder(Seat::NORTH), SeatOrder(Seat::SOUTH)};
    }

    static std::array<std::vector<Card>, NUM_SEATS> getHands(const Spades &spades, const SeatOrder &seatOrder)
    {
        return {spades.getHand(seatOrder.seat1), spades.getHand(seatOrder.seat2), spades.getHand(seatOrder.seat3), spades.getHand(seatOrder.seat4)};
    }
};

class AnalyzeTest : public ::testing::Test
{
protected:
    Spades spades;
    Analyze analyze;
    std::map<Seat, std::set<Card>> playedCards;

    AnalyzeTest() : analyze(spades)
    {
    }

    void SetUp() override
    {
        for (const auto &seat : SeatUtils::getSeats())
        {
            playedCards[seat] = {};
        }
        spades.reset(BidVariationType::DOUBLE_NILL, TrumpVariationType::ACE_HIGH);
        spades.reset(0);
        spades.addBid(3);
        spades.addBid(3);
        spades.addBid(3);
        spades.addBid(0);
    }

public:
    std::optional<Card> getPlayableCard()
    {
        for (const auto &card : spades.getHand(spades.getTurnSeat()))
        {
            if (spades.canPlace(card))
            {
                return std::make_optional(card);
            }
        }
        return std::nullopt;
    }

    bool containsCard(const std::vector<Card> &cards, const Card &card) const
    {
        return std::any_of(cards.begin(), cards.end(),
                           [&card](const Card &handCard)
                           { return card == handCard; });
    }
    bool containsCard(const std::vector<std::vector<Card>> &cardContainers, const Card &card) const
    {
        for (const auto &cards : cardContainers)
        {
            bool hasCard = std::any_of(cards.begin(), cards.end(),
                                       [&card](const Card &handCard)
                                       { return card == handCard; });
            if (hasCard)
            {
                return true;
            }
        }
        return false;
    }

    Card getAnyPlaceableCard()
    {
        for (const auto card : spades.getHand(spades.getTurnSeat()))
        {
            if (spades.canPlace(card))
            {
                return card;
            }
        }
        assert(false);
        return Card();
    }

    Card placeAnyCard()
    {
        Card card = getAnyPlaceableCard();
        spades.place(card);
        return card;
    }

    void assertEffectiveSuitFromElimination(const Seat &seat1, const Seat &seat2,
                                            const Seat &seat3, const Seat &seat4) const;
};

void AnalyzeTest::assertEffectiveSuitFromElimination(const Seat &perspectiveSeat, const Seat &seat2,
                                                     const Seat &seat3, const Seat &seat4) const
{
    const auto voidSuits1 = analyze.getUnfollowedEffectiveLeadSuits(seat2);
    const auto voidSuits2 = analyze.getUnfollowedEffectiveLeadSuits(seat3);
    const auto voidSuits3 = analyze.getUnfollowedEffectiveLeadSuits(seat4);

    const auto knownSuits1 = analyze.getEffectiveSuitsFromElimination(seat2);
    const auto knownSuits2 = analyze.getEffectiveSuitsFromElimination(seat3);
    const auto knownSuits3 = analyze.getEffectiveSuitsFromElimination(seat4);

    for (const auto &suit : {Suit::SPADE, Suit::HEART, Suit::DIAMOND, Suit::CLOVER})
    {
        if (analyze.isEffectiveSuitInOtherHand(perspectiveSeat, suit))
        {
            ASSERT_EQ((voidSuits1.contains(suit) && voidSuits2.contains(suit)), knownSuits3.contains(suit));
            ASSERT_EQ((voidSuits2.contains(suit) && voidSuits3.contains(suit)), knownSuits1.contains(suit));
            ASSERT_EQ((voidSuits1.contains(suit) && voidSuits3.contains(suit)), knownSuits2.contains(suit));
        }
    }
}

TEST_F(AnalyzeTest, GetEffectiveSuitsFromElimination)
{
    const Seat selfSeat = Seat::SOUTH;
    const auto otherSeats = SeatUtils::getOtherSeats(selfSeat);
    for (int i = 0; i < DECK_SIZE - 1; i++)
    {
        placeAnyCard();
        assertEffectiveSuitFromElimination(selfSeat, otherSeats[0], otherSeats[1], otherSeats[2]);
        assertEffectiveSuitFromElimination(selfSeat, otherSeats[0], otherSeats[2], otherSeats[1]);
        assertEffectiveSuitFromElimination(selfSeat, otherSeats[1], otherSeats[2], otherSeats[0]);
    }
}

TEST_F(AnalyzeTest, GetUnfollowedEffectiveLeadSuits)
{
    for (const auto &targetSeat : SeatUtils::getSeats())
    {
        spades.reset();
        std::set<Suit> voidSuits;
        for (int i = 0; i < HAND_SIZE; i++)
        {
            const auto leadSuit = spades.getEffectiveSuit(placeAnyCard());
            for (int j = 1; j < NUM_SEATS; j++)
            {
                const auto followSeat = spades.getTurnSeat();
                const auto followSuit = spades.getEffectiveSuit(placeAnyCard());
                if (followSeat == targetSeat && followSuit != leadSuit)
                {
                    voidSuits.insert(leadSuit);
                    ASSERT_EQ(voidSuits.size(), analyze.getUnfollowedEffectiveLeadSuits(targetSeat).size()) << (int)targetSeat << " " << i << " " << j;
                }
            }
        }
    }
}

TEST_F(AnalyzeTest, GetPlayedSeatRoundCards)
{

    for (int i = 0; i < HAND_SIZE; i++)
    {
        for (int j = 0; j < NUM_SEATS; j++)
        {
            const auto seat = spades.getTurnSeat();
            playedCards[seat].insert(placeAnyCard());

            if (spades.getCurrentRoundCardSeatPairs().size() % DECK_SIZE != 0)
            {
                ASSERT_EQ(analyze.getPlayedRoundCards(seat).size(), playedCards[seat].size());
            }
            else
            {
                ASSERT_EQ(analyze.getPlayedRoundCards(seat).size(), 0);
            }

            const SeatOrder seatOrder(seat);
            for (const auto &card : analyze.getPlayedRoundCards(seat))
            {
                ASSERT_TRUE(playedCards[seat].contains(card));
                for (const auto &otherSeat : seatOrder.getOtherSeats())
                {
                    ASSERT_FALSE(playedCards[otherSeat].contains(card));
                }
            }
        }
    }
}

TEST_F(AnalyzeTest, GetRemainingNonSeatCards)
{
    for (const auto &seatOrder : SeatOrder::getSeatOrders())
    {
        const auto hands = SeatOrder::getHands(spades, seatOrder);
        const auto remainingNonSeatCards = analyze.getRemainingNonSeatRoundCards(seatOrder.seat1);
        ASSERT_EQ(remainingNonSeatCards.size(), DECK_SIZE - HAND_SIZE);
        for (const auto &card : remainingNonSeatCards)
        {
            ASSERT_FALSE(containsCard(hands[0], card));
            ASSERT_TRUE(containsCard({hands[1], hands[2], hands[3]}, card));
        }
    }
}
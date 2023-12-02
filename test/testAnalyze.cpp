#include "test_pch.h"
#include "spades/Spades.h"
#include "spades_ai/Analyze.h"

using namespace spd;

class AnalyzeTest : public ::testing::Test
{
protected:
    Spades spades;
    Analyze analyze;

    AnalyzeTest() : analyze(spades) {

    }

    void SetUp() override
    {
        spades.reset(BidVariationType::DOUBLE_NILL, TrumpVariationType::ACE_HIGH);
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
};

TEST_F(AnalyzeTest, GetRemainingNonSeatCardsStart)
{
    for (const auto &seat1 : SeatUtils::getSeats())
    {
        const auto seat2 = SeatUtils::getNextSeat(seat1);
        const auto seat3 = SeatUtils::getNextSeat(seat2);
        const auto seat4 = SeatUtils::getNextSeat(seat3);

        const auto hand1 = spades.getHand(seat1);
        const auto hand2 = spades.getHand(seat2);
        const auto hand3 = spades.getHand(seat3);
        const auto hand4 = spades.getHand(seat4);

        const auto remainingNonSeatCards = analyze.getRemainingNonSeatCards(seat1);
        ASSERT_EQ(remainingNonSeatCards.size(), Deck::DECK_SIZE - Deck::HAND_SIZE);

        for (const auto &card : remainingNonSeatCards)
        {
            ASSERT_FALSE(containsCard(hand1, card));
            ASSERT_TRUE(containsCard({hand2, hand3, hand4}, card));
        }
    }
}
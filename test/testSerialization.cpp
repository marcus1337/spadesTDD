#include "test_pch.h"
#include "spades/Spades.h"
#include "spades/data/card/Card.h"
#include "spades/history/SpadesHistory.h"

using namespace spd;

TEST(Serialization, Seed)
{
    Spades spades;
    const auto data = spades.serialize();
    const unsigned int oldSeed = spades.getSeed();
    spades.reset(oldSeed + 1);
    spades.deserialize(data);
    EXPECT_EQ(spades.serialize(), data);
    EXPECT_EQ(spades.getSeed(), oldSeed);
    Spades spades2;
    EXPECT_NE(spades2.getSeed(), oldSeed);
}

class CardSerializationTest : public ::testing::Test
{
protected:
    std::vector<int> serializedCards;
    void SetUp() override
    {
        for (const auto &card : Card::getCards())
        {
            serializedCards.push_back(card.serialize());
        }
    }
};

TEST_F(CardSerializationTest, SerializeCards)
{
    std::unordered_set<int> uniqueValues(serializedCards.begin(), serializedCards.end());
    ASSERT_EQ(serializedCards.size(), uniqueValues.size()) << "Duplicate serialization values found!";
}

TEST_F(CardSerializationTest, DeserializeCards)
{
    std::vector<Card> deserializedCards;
    for (const auto &value : serializedCards)
    {
        deserializedCards.push_back(Card(value));
    }
    std::vector<Card> allCards = Card::getCards();
    ASSERT_EQ(allCards.size(), deserializedCards.size()) << "Number of cards mismatch.";
    for (size_t i = 0; i < allCards.size(); ++i)
    {
        ASSERT_EQ(allCards[i], deserializedCards[i]) << "Cards at index " << i << " are not the same.";
    }
}

TEST(Serialization, HistoryUndo)
{

    Spades spades;
    spades.addBid(1);
    const auto encoding = spades.serialize();
    spades.reset();
    spades.deserialize(encoding);
    ASSERT_TRUE(spades.serialize() == encoding) << spades.serialize() << " != " << encoding;
    ASSERT_TRUE(spades.canUndo());

    auto tmpTest = spades.serialize();

    spades.undo();

    spades.redo();
    EXPECT_TRUE(encoding == spades.serialize()) << spades.serialize() << " != " << encoding;
}

TEST(Serialization, HistorySimple)
{
    /*   Spades spades;
      for (int i = 0; i < 4; i++)
          spades.addBid(1);
      const auto encoding = spades.serialize();
      spades.reset();
      spades.deserialize(encoding); */
}

TEST(Serialization, History)
{

    Spades spades;
    const auto c1a = Card(Rank::ACE, Suit::DIAMOND);
    const auto c2a = Card(Rank::TWO, Suit::CLOVER);
    for (int i = 0; i < 4; i++)
        spades.addBid(1);
    spades.place(c1a);
    spades.place(c2a);
    spades.undo();
    spades.undo();
    const auto encoding = spades.serialize();
    spades.reset();
    spades.deserialize(encoding); 
    ASSERT_TRUE(spades.canRedo());
    spades.redo();
    ASSERT_TRUE(spades.canRedo());
    spades.redo();
    const auto playedTrickSeatCardPairs = spades.getCurrentTrickCardSeatPairs();
    ASSERT_EQ(playedTrickSeatCardPairs.size(), 2);
    const auto c1b = playedTrickSeatCardPairs[0].second;
    const auto c2b = playedTrickSeatCardPairs[1].second;
    EXPECT_EQ(c1a, c1b) << "TEST A ";
    EXPECT_EQ(c2a, c2b) << "TEST B ";
}

#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>
#include <unordered_set>
#include "data/card/Card.h"
#include "history/SpadesHistory.h"
#include <sstream>

using namespace spd;


TEST(Serialization, Seed) {
    Spades spades;
    const auto data = spades.serialize();
    const unsigned int oldSeed = spades.getSeed();
    spades.reset(oldSeed+1);
    spades.deserialize(data);
    EXPECT_EQ(spades.serialize(), data);
    EXPECT_EQ(spades.getSeed(), oldSeed);
    Spades spades2;
    EXPECT_NE(spades2.getSeed(), oldSeed);
}

class CardSerializationTest : public ::testing::Test {
protected:
    std::vector<int> serializedCards;
    void SetUp() override {
        for (const auto& card : Card::getCards()) {
            serializedCards.push_back(card.serialize());
        }
    }
};

TEST_F(CardSerializationTest, SerializeCards) {
    std::unordered_set<int> uniqueValues(serializedCards.begin(), serializedCards.end());
    ASSERT_EQ(serializedCards.size(), uniqueValues.size()) << "Duplicate serialization values found!";
}

TEST_F(CardSerializationTest, DeserializeCards) {
    std::vector<Card> deserializedCards;
    for (const auto& value : serializedCards) {
        deserializedCards.push_back(Card(value));
    }
    std::vector<Card> allCards = Card::getCards();
    ASSERT_EQ(allCards.size(), deserializedCards.size()) << "Number of cards mismatch.";
    for (size_t i = 0; i < allCards.size(); ++i) {
        ASSERT_EQ(allCards[i], deserializedCards[i]) << "Cards at index " << i << " are not the same.";
    }
}

TEST(Serialization, History){

    Spades spades;
    const auto c1a = Card(Rank::ACE, Suit::DIAMOND);
    const auto c2a = Card(Rank::TWO, Suit::CLOVER);
    for(int i = 0; i < 4; i++)
        spades.addBid(1);
    spades.playCard(c1a);
    spades.playCard(c2a);
    spades.undo();
    spades.undo();
    const auto encoding = spades.serialize();
    spades.reset();
    spades.deserialize(encoding);
    ASSERT_TRUE(spades.canRedo());
    spades.redo();
    ASSERT_TRUE(spades.canRedo());
    spades.redo();
    const auto playedTrickSeatCardPairs = spades.getPlayedTrickSeatCardPairs();
    ASSERT_EQ(playedTrickSeatCardPairs.size(), 2);
    const auto c1b = playedTrickSeatCardPairs[0].second;
    const auto c2b = playedTrickSeatCardPairs[1].second;
    EXPECT_EQ(c1a, c1b);
    EXPECT_EQ(c2a, c2b);
}


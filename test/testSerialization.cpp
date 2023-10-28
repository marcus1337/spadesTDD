#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include <algorithm>
#include <set>
#include <unordered_set>
#include "data/card/Card.h"

using namespace spd;


TEST(Serialization, Seed) {
    Spades spades;
    const auto data = spades.serialize();
    int oldSeed = spades.getSeed();
    spades.setSeed(spades.getSeed()+1);
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

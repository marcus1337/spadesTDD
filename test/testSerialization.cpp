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
    SpadesHistory history;
    const auto c1a = Card(Rank::ACE, Suit::DIAMOND);
    const auto c2a = Card(Rank::TWO, Suit::CLOVER);
    history.addCommand(std::make_unique<PlaceCommand>(c1a));
    history.addCommand(std::make_unique<PlaceCommand>(c2a));
    std::vector<int> encodedValues;
    std::istringstream iss(history.serialize());
    int num;
    while (iss >> num) {
        encodedValues.push_back(num);
    }
    ASSERT_TRUE(encodedValues.size() == 4);
    Card c1b(encodedValues[1]);
    Card c2b(encodedValues[3]);
    EXPECT_EQ(c1a, c1b);
    EXPECT_EQ(c2a, c2b);
}
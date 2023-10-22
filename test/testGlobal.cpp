#include <vector>
#include <gtest/gtest.h>
#include "Spades.h"
#include "global/PortableRandom.h"
#include <algorithm>
#include "Deck.h"
#include <set>

using namespace spd;

TEST(PortableRandom, RandomSame) {
    auto& r = PortableRandom::getInstance();
    std::vector<int> arr1, arr2;
    const int n = 200;
    const int maxValue = 100;
    const int seed = 100;

    r.setSeed(seed);
    for (int i = 0; i < n; i++) {
        arr1.push_back(r.randInt(0, maxValue));
    }
    r.setSeed(seed);
    for (int i = 0; i < n; i++) {
        arr2.push_back(r.randInt(0, maxValue));
    }
    bool areEqual = std::equal(arr1.begin(), arr1.end(), arr2.begin());

    EXPECT_TRUE(areEqual);
}

TEST(PortableRandom, RandomDiff) {
    auto& r = PortableRandom::getInstance();
    std::vector<int> arr1, arr2;
    const int n = 200;
    const int maxValue = 100;
    const int seed1 = 100;
    const int seed2 = seed1+1;

    r.setSeed(seed1);
    for (int i = 0; i < n; i++) {
        arr1.push_back(r.randInt(0, maxValue));
    }
    r.setSeed(seed2);
    for (int i = 0; i < n; i++) {
        arr2.push_back(r.randInt(0, maxValue));
    }
    bool areEqual = std::equal(arr1.begin(), arr1.end(), arr2.begin());

    EXPECT_FALSE(areEqual);
}
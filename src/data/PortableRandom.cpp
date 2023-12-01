#include "spades/data/PortableRandom.h"

using namespace spd;

PortableRandom::PortableRandom() : seed(makeRandomSeed()), mt(seed)
{
}

unsigned int PortableRandom::makeRandomSeed()
{
    return (unsigned int)std::random_device()();
}

void PortableRandom::setSeed(unsigned int newSeed)
{
    seed = newSeed;
    mt = std::mt19937(seed);
}

unsigned int PortableRandom::getSeed() const
{
    return seed;
}

int PortableRandom::randInt(int from, int to)
{
    custom_uniform_int_distribution<int> dis(from, to);
    return dis(mt);
}
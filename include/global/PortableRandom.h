#pragma once
#include <random>
#include <iostream>
#include <vector>

namespace spd {

    template <class IntType = int>
    struct custom_uniform_int_distribution //Partially from https://stackoverflow.com/a/44525591/7202012
    {
        using result_type = IntType;
        const result_type from, to;
        explicit custom_uniform_int_distribution(const result_type from = 0, const result_type to = std::numeric_limits<result_type>::max())
            : from(from), to(to)
        {}
        template <class Generator>
        result_type operator()(Generator& g) const
        {
            return rnd(g, from, to);
        }
        template <class Generator>
        result_type operator()(Generator& g, const result_type& from, const result_type& to) const
        {
            return rnd(g, from, to);
        }

        template <class Generator>
        result_type reject_sampling(Generator& g, const result_type reject_lim) const
        {
            result_type n = g();
            while (n <= reject_lim)
                n = g();
            return n;
        }

    private:
        template <class Generator>
        result_type rnd(Generator& g, const result_type a, const result_type b) const
        {
            static_assert(std::is_convertible<typename Generator::result_type, result_type>::value, "Not conver");
            static_assert(Generator::min() == 0, "If non-zero we have handle the offset");
            const auto range = b - a + 1;
            auto n = reject_sampling(g, g.max() % range);
            return (n % range) + a;
        }
    };

    template<class RandomIt, class UniformRandomBitGenerator>
    void custom_shuffle(RandomIt first, RandomIt last, UniformRandomBitGenerator&& g)
    {
        typedef typename std::iterator_traits<RandomIt>::difference_type diff_t;
        typedef custom_uniform_int_distribution<diff_t> distr_t;
        distr_t D;
        diff_t n = last - first;
        for (diff_t i = n - 1; i > 0; --i)
        {
            std::swap(first[i], first[D(g, 0, i)]);
        }
    }

    class PortableRandom {

        int seed;
        std::mt19937 mt;
        PortableRandom() : seed(std::random_device()()), mt(seed) {
        }

    public:

        static PortableRandom& getInstance() {
            static PortableRandom instance;
            return instance;
        }

        void setSeed(unsigned int newSeed) {
            seed = newSeed;
            mt = std::mt19937(seed);
        }

        unsigned int getSeed() const {
            return seed;
        }

        int randInt(int from, int to) {
            custom_uniform_int_distribution<int> dis(from, to);
            return dis(mt);
        }

    };
}
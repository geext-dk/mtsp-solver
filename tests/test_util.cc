#include <gtest/gtest.h>
#include <unordered_map>

#include "util.h"
#include "adjacency_list.h"

using namespace mtsp;


TEST(TestUtil, Dijkstra) {
    std::cout << std::endl;
    AdjacencyList list({
            {0, {EndVertex(1, 5), EndVertex(2, 7)}},
            {1, {EndVertex(0, 5), EndVertex(3, 9)}},
            {2, {EndVertex(0, 7), EndVertex(3, 1)}},
            {3, {EndVertex(1, 9), EndVertex(2, 1)}}
    });
    std::unordered_map<std::size_t, unsigned long> distance;
    std::unordered_map<std::size_t, std::size_t> previous;
    std::tie(distance, previous) = dijkstra(list, 0);
    ASSERT_EQ(0, distance[0]);
    ASSERT_EQ(0, previous[0]);

    ASSERT_EQ(5, distance[1]);
    ASSERT_EQ(0, previous[1]);

    ASSERT_EQ(7, distance[2]);
    ASSERT_EQ(0, previous[2]);

    ASSERT_EQ(8, distance[3]);
    ASSERT_EQ(2, previous[3]);
}

TEST(TestUtil, FirstFreeRandom) {
    // 20 items, 10 zeroes
    unsigned long number_of_free = 10;
    std::vector<unsigned> is_assigned = { 1, 1, 0, 0, 1, 0, 0, 1, 0, 1,
                                          1, 1, 0, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 10; ++i) {
        unsigned long random_position = firstFreeRandom(is_assigned,
                                                        number_of_free);
        ASSERT_LT(random_position, is_assigned.size());
        ASSERT_GE(random_position, 0);
        ASSERT_EQ(0, is_assigned.at(random_position));
        --number_of_free;
    }
}

TEST(TestUtil, GetRandomInteger) {
    std::size_t begin = 1;
    std::size_t end = 100;
    std::size_t min = end;
    std::size_t max = begin;
    for (unsigned i = 0; i < 10000; ++i) {
        std::size_t random_int = getRandomInteger<std::size_t>(begin, end);
        if (random_int > max) {
            max = random_int;
        }
        if (random_int < min) {
            min = random_int;
        }
    }

    ASSERT_EQ(begin, min);
    ASSERT_EQ(end - 1, max);
}

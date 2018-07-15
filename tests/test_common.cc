//
// Created by geext on 7/8/18.
//

#include <gtest/gtest.h>
#include <unordered_map>

#include "common.h"


TEST(TestCommon, Dijkstra) {
    std::cout << std::endl;
    std::unordered_map<std::size_t, std::vector<mtsp::EndVertex>> adjacency_list {
            {0, { mtsp::EndVertex(1, 5),  mtsp::EndVertex(2, 7)}},
            {1, { mtsp::EndVertex(0, 5),  mtsp::EndVertex(3, 9)}},
            {2, { mtsp::EndVertex(0, 7),  mtsp::EndVertex(3, 1)}},
            {3, { mtsp::EndVertex(1, 9),  mtsp::EndVertex(2, 1)}}
    };
    std::unordered_map<std::size_t, std::size_t> distance, previous;
    std::tie(distance, previous) = mtsp::dijkstra(adjacency_list, 0);
    ASSERT_EQ(0, distance[0]);
    ASSERT_EQ(0, previous[0]);

    ASSERT_EQ(5, distance[1]);
    ASSERT_EQ(0, previous[1]);

    ASSERT_EQ(7, distance[2]);
    ASSERT_EQ(0, previous[2]);

    ASSERT_EQ(8, distance[3]);
    ASSERT_EQ(2, previous[3]);
}

TEST(TestCommon, FirstFreeRandom) {
    // 20 items, 10 zeroes
    unsigned long number_of_free = 10;
    std::vector<unsigned> is_assigned = { 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 10; ++i) {
        unsigned long random_position = mtsp::firstFreeRandom(is_assigned, number_of_free);
        ASSERT_LT(random_position, is_assigned.size());
        ASSERT_GE(random_position, 0);
        ASSERT_EQ(0, is_assigned.at(random_position));
        --number_of_free;
    }
}


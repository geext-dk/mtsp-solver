//
// Created by geext on 7/8/18.
//

#include <gtest/gtest.h>
#include <unordered_map>

#include "common.h"


TEST(Common, Dijkstra) {
    std::cout << std::endl;
    std::unordered_map<int, std::vector<mtsp::Edge>> adjacency_list {
            {0, { mtsp::Edge(1, 5),  mtsp::Edge(2, 7)}},
            {1, { mtsp::Edge(0, 5),  mtsp::Edge(3, 9)}},
            {2, { mtsp::Edge(0, 7),  mtsp::Edge(3, 1)}},
            {3, { mtsp::Edge(1, 9),  mtsp::Edge(2, 1)}}
    };
    std::unordered_map<int, int> distance, previous;
    std::tie(distance, previous) = mtsp::dijkstra(adjacency_list, 0);
    ASSERT_EQ(0, distance[0]);
    ASSERT_EQ(-1, previous[0]);

    ASSERT_EQ(5, distance[1]);
    ASSERT_EQ(0, previous[1]);

    ASSERT_EQ(7, distance[2]);
    ASSERT_EQ(0, previous[2]);

    ASSERT_EQ(8, distance[3]);
    ASSERT_EQ(2, previous[3]);
}

TEST(Common, FirstFreeRandom) {
    // 20 items, 10 zeroes
    int number_of_free = 10;
    std::vector<int> assigned = { 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 10; ++i) {
        int random_position = mtsp::firstFreeRandom(assigned, number_of_free);
        ASSERT_LT(random_position, assigned.size());
        ASSERT_GE(random_position, 0);
        ASSERT_EQ(0, assigned.at(random_position));
        --number_of_free;
    }
}


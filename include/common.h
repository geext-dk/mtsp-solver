#ifndef MTSPSOLVER_COMMON_H
#define MTSPSOLVER_COMMON_H

#include <random>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <iostream>
#include <chrono>

#include "end_vertex.h"
#include "destination.h"
#include "adjacency_list.h"

namespace mtsp {

    // Thanks to the function we create the random device only once
    std::default_random_engine& getRandomEngine();

    template<typename T>
    T getRandomInteger(T first, T second) {
        std::uniform_int_distribution<T> uniform_dist(first, second - 1);
        return uniform_dist(getRandomEngine());
    }

    // generate randomized index that is guaranteed to be not assigned before
    std::size_t firstFreeRandom(const std::vector<unsigned> &assigned, std::size_t number_of_free);

    // dijkstra's algorithm implementation
    std::tuple<std::unordered_map<std::size_t, std::size_t>, std::unordered_map<std::size_t, std::size_t>>
    dijkstra(const AdjacencyList &adjacency_list, std::size_t from);
}

#endif  // MTSPSOLVER_COMMON_H

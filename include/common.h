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

namespace mtsp {

    // Thanks to the function we create the random device only once
    std::default_random_engine& getRandomEngine();

    // generate randomized index that is guaranteed to be not assigned before
    unsigned long firstFreeRandom(const std::vector<int> &assigned, unsigned long number_of_free);

    // dijkstra's algorithm implementation
    std::tuple<std::unordered_map<int, int>, std::unordered_map<int, int>>
    dijkstra(const std::unordered_map<int, std::vector<EndVertex>> &adjacancy_list, int from);
}

#endif  // MTSPSOLVER_COMMON_H

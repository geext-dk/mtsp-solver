#ifndef MTSPSOLVER_COMMON_H
#define MTSPSOLVER_COMMON_H

#include <random>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <iostream>

#include "edge.h"
namespace mtsp {
    // generate randomized index that is guaranted to be not assigned before
    unsigned long firstFreeRandom(std::vector<int> &assigned, unsigned long number_of_free);

    // dijkstra's algorithm implementation
    std::tuple<std::unordered_map<int, int>, std::unordered_map<int, int>>
    dijkstra(std::unordered_map<int, std::vector<Edge>> &adjacancy_list, int from);
}

#endif  // MTSP_COMMON_H

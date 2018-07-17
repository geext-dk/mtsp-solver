//
// Created by geext on 17.07.18.
//

#ifndef MTSPSOLVERMAIN_ADJACENCYLIST_H
#define MTSPSOLVERMAIN_ADJACENCYLIST_H

#include <vector>
#include <unordered_map>

#include "end_vertex.h"

class AdjacencyList {
public:
    const std::vector<mtsp::EndVertex> &getEdges(std::size_t vertex) const;
    void addEdge(std::size_t first_vertex, std::size_t second_vertex, unsigned long weight);

private:
    std::unordered_map<std::size_t, std::vector<mtsp::EndVertex>> _data;
};


#endif //MTSPSOLVERMAIN_ADJACENCYLIST_H

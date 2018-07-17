//
// Created by geext on 17.07.18.
//

#include "adjacency_list.h"


const std::vector<mtsp::EndVertex> &AdjacencyList::getEdges(std::size_t vertex) const {
    return _data[vertex];
}

void AdjacencyList::addEdge(std::size_t first_vertex, std::size_t second_vertex, unsigned long weight) {
    _data[first_vertex].emplace_back(second_vertex, weight);
    _data[second_vertex].emplace_back(first_vertex, weight);
}
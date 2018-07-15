//
// Created by geext on 7/8/18.
//

#include "graph.h"
namespace mtsp {
    Graph::Graph(const std::vector<std::vector<unsigned long>> &adjacency_matrix)
            : _adjacency_matrix(adjacency_matrix), _number_of_vertices(adjacency_matrix.size())
    {
    }

    std::size_t Graph::getNumberOfVertices() const
    {
        return _number_of_vertices;
    }

    unsigned long Graph::getWeight(std::size_t first_vertex, std::size_t second_vertex) const
    {
        return _adjacency_matrix[first_vertex][second_vertex];
    }
}
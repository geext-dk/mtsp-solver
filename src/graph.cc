//
// Created by geext on 7/8/18.
//

#include "graph.h"
namespace mtsp {
    Graph::Graph(const std::vector<std::vector<int>> &adjacency_matrix)
            : _adjacency_matrix(adjacency_matrix), _number_of_vertices(adjacency_matrix.size())
    {
    }

    unsigned long Graph::getNumberOfVertices() const
    {
        return _number_of_vertices;
    }

    int Graph::getWeight(int first_vertex, int second_vertex) const
    {
        return _adjacency_matrix[first_vertex][second_vertex];
    }
}
//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_GRAPH_H
#define MTSPSOLVER_GRAPH_H
#include <vector>

namespace mtsp {
    class Graph {

    public:
        explicit Graph(const std::vector<std::vector<unsigned long>> &matrix);
        unsigned long getNumberOfVertices() const;
        unsigned long getWeight(std::size_t first_vertex, std::size_t second_vertex) const;

    private:
        const std::vector<std::vector<std::size_t>> _adjacency_matrix;
        const std::size_t _number_of_vertices;
    };
}


#endif //MTSPSOLVER_GRAPH_H

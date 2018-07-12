//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_GRAPH_H
#define MTSPSOLVER_GRAPH_H
#include <vector>

namespace mtsp {
    class Graph {

    public:
        explicit Graph(const std::vector<std::vector<int>> &matrix);
        unsigned long getNumberOfVertices() const;
        int getWeight(int first_vertex, int second_vertex) const;

    private:
        const std::vector<std::vector<int>> _adjacency_matrix;
        const unsigned long _number_of_vertices;
    };
}


#endif //MTSPSOLVER_GRAPH_H

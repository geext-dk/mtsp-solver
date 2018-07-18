//
// Created by geext on 18.07.18.
//

#include <stdexcept>
#include "solver_graph.h"

namespace mtsp {
    void SolverGraph::setNumberOfVertices(std::size_t n) {
        _matrix.clear();
        _matrix.resize(n, std::vector<unsigned long>(n, 0));
    }

    unsigned long SolverGraph::getWeight(std::size_t v1, std::size_t v2) const {
        if (v1 >= _matrix.size() || v2 >= _matrix.size()) {
            throw std::invalid_argument("The vertex number exceeds tha matrix bounds");
        }
        return _matrix[v1][v2];
    }

    void SolverGraph::setWeight(std::size_t v1, std::size_t v2, unsigned long w) {
        if (v1 >= _matrix.size() || v2 >= _matrix.size()) {
            throw std::invalid_argument("The vertex number exceeds tha matrix bounds");
        }
        _matrix[v1][v2] = _matrix[v2][v1] = w;
    }

    std::size_t SolverGraph::getNumberOfVertices() const {
        return _matrix.size();
    }
}
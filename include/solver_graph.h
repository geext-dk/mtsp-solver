//
// Created by geext on 18.07.18.
//

#ifndef MTSPSOLVERMAIN_SOLVER_GRAPH_H
#define MTSPSOLVERMAIN_SOLVER_GRAPH_H
#include <vector>
#include "igraph.h"

namespace mtsp {
    class SolverGraph : IGraph {
    public:
        unsigned long getWeight(std::size_t v1, std::size_t v2) const override;
        void setWeight(std::size_t v1, std::size_t v2, unsigned long w) override;
        std::size_t getNumberOfVertices() const override;
        void setNumberOfVertices(std::size_t n);

    private:
        std::vector<std::vector<unsigned long>> _matrix;
    };
}


#endif //MTSPSOLVERMAIN_SOLVER_GRAPH_H

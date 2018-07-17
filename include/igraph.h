//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_GRAPH_H
#define MTSPSOLVER_GRAPH_H
#include <vector>
#include <stdexcept>
#include <memory>

namespace mtsp {
    class IGraph {
    public:
        virtual unsigned long getWeight(std::size_t v1, std::size_t v2) const = 0;
        virtual void setWeight(std::size_t v1, std::size_t v2, unsigned long w) = 0;
        virtual std::unique_ptr<IGraph> copy() const = 0;
        virtual std::size_t getNumberOfVertices() const = 0;
    };
}


#endif //MTSPSOLVER_GRAPH_H

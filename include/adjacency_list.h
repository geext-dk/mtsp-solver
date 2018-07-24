#ifndef MTSPSOLVER_ADJACENCY_LIST_H
#define MTSPSOLVER_ADJACENCY_LIST_H
#include <cstddef>
#include <vector>
#include <unordered_map>

#include "end_vertex.h"

namespace mtsp {

class AdjacencyList {
public:
    AdjacencyList() = default;
    explicit AdjacencyList(const std::unordered_map<std::size_t,
                            std::vector<EndVertex>> &adj_list);
    const std::vector<EndVertex> &getIncidentEdges(std::size_t v) const;
    void addEdge(std::size_t v1, std::size_t v2, unsigned long w);

private:
    std::unordered_map<std::size_t, std::vector<EndVertex>> _data;
};

}
#endif  // MTSPSOLVER_ADJACENCY_LIST_H

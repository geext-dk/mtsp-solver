#include "adjacency_list.h"

namespace mtsp {

void AdjacencyList::addEdge(std::size_t v1, std::size_t v2, unsigned long w) {
    for (auto end_vertex : _data[v1]) {
        if (end_vertex.vertex == v2) {
            return;
        }
    }
    _data[v1].emplace_back(v2, w);
    _data[v2].emplace_back(v1, w);
}

AdjacencyList::AdjacencyList(const std::unordered_map<std::size_t,
                        std::vector<EndVertex>> &adj_list) 
        : _data(adj_list) { }
    
const std::vector<EndVertex> &AdjacencyList::getIncidentEdges(
                                                        std::size_t v) const {
    return _data.at(v);
}

AdjacencyList::iterator AdjacencyList::begin() const {
    return _data.cbegin();
}

AdjacencyList::iterator AdjacencyList::end() const {
    return _data.cend();
}

}

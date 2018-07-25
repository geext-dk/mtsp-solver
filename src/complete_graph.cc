#include "complete_graph.h"

#include <stdexcept>

namespace mtsp {

CompleteGraph::CompleteGraph(std::size_t n)
        : _data(n, std::vector<std::size_t>(n, 0)) { }

void CompleteGraph::setNumberOfVertices(std::size_t n) {
    _data.clear();
    _data.resize(n, std::vector<unsigned long>(n, 0));
}

unsigned long CompleteGraph::getWeight(std::size_t v1, std::size_t v2) 
                                                                        const {
    return _data.at(v1).at(v2);
}

void CompleteGraph::setWeight(std::size_t v1, std::size_t v2,
                                   unsigned long w) {

    _data.at(v1).at(v2) = _data.at(v2).at(v1) = w;
}

void CompleteGraph::resize(std::size_t n) {
    _data.clear();
    _data.resize(n, std::vector<unsigned long>(n, 0));
}

}

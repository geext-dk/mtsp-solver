#ifndef MTSPSOLVER_COMPLETE_GRAPH_H
#define MTSPSOLVER_COMPLETE_GRAPH_H
#include <cstddef>
#include <vector>

namespace mtsp {

class CompleteGraph {
public:
    explicit CompleteGraph(std::size_t n);
    CompleteGraph() = default;

    void setNumberOfVertices(std::size_t n);
    unsigned long getWeight(std::size_t v1, std::size_t v2) const;
    void setWeight(std::size_t v1, std::size_t v2, unsigned long w);
    void resize(std::size_t n);

private:
    std::vector<std::vector<unsigned long>> _data;
};

}

#endif  // MTSPSOLVER_COMPLETE_GRAPH_H

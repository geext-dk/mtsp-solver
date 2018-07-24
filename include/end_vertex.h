#ifndef MTSPSOLVER_EDGE_H
#define MTSPSOLVER_EDGE_H
#include <cstddef>

namespace mtsp {

struct EndVertex {
     EndVertex(std::size_t v, unsigned long w);
     std::size_t vertex;
     unsigned long weight;
};

}

#endif  // MTSPSOLVER_EDGE_H

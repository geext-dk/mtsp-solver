#ifndef MTSPSOLVER_DESTINATION_H
#define MTSPSOLVER_DESTINATION_H
#include <cstddef>

namespace mtsp {
// The struct represents destination from first vertex to the "destination" vertex
// in the Dijkstra algorithm implementation
struct Destination {
    Destination(std::size_t dest, unsigned long dist);
    std::size_t destination;
    unsigned long distance;
};

}

#endif  // MTSPSOLVER_DESTINATION_H

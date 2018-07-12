#ifndef MTSPSOLVER_EDGE_H
#define MTSPSOLVER_EDGE_H
namespace mtsp {

    struct EndVertex {
        EndVertex(int v, int w);
        int vertex;
        int weight;
    };
}

#endif  // MTSPSOLVER_EDGE_H
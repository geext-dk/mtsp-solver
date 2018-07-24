#ifndef MTSPSOLVER_MTSP_SOLVER_H
#define MTSPSOLVER_MTSP_SOLVER_H
#include <cstddef>
#include <tuple>
#include <vector>

#include "complete_graph.h"
#include "adjacency_list.h"
#include "solution.h"
#include "util.h"

namespace mtsp {

class MtspSolver {
public:

    void importAdjacencyList(const AdjacencyList &adjacency_list,
                             std::vector<std::size_t> &vertices,
                             std::size_t origin,
                             std::size_t number_of_chains);

    void solve();

    const CompleteGraph &getInternalGraph() const;

private:
    unsigned long MaximumSameResults = 10000;
    unsigned long InitialNumber = 1;
    unsigned long MutationNumber = InitialNumber * 4;
    unsigned long CrossoverNumber = InitialNumber * 2;
    unsigned long SelectionGroupSize =
                    (InitialNumber + MutationNumber + CrossoverNumber)
                    / InitialNumber;

    unsigned long objectiveFunction(const Solution &solution);
    void generateInitialPopulation();
    void crossover();
    void mutation();
    std::tuple<std::size_t, unsigned long> selection();
    std::tuple<CompleteGraph, std::vector<unsigned long>>
    convertToCompleteGraph(const AdjacencyList &adjacency_list,
                           const std::vector<std::size_t> &vertices,
                           const std::size_t origin_vertex);

    CompleteGraph _graph;
    std::size_t _global_origin = 0;
    std::size_t _number_of_chains = 0;
    std::size_t _number_of_vertices = 0;
    std::vector<std::size_t> _global_vertices;
    std::vector<unsigned long> _origin_distance;
    std::vector<Solution> _population;
    bool _ready = false;
};

}

#endif // MTSPSOLVER_MTSP_SOLVER_H

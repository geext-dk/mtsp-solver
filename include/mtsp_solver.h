//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_MTSP_SOLVER_H
#define MTSPSOLVER_MTSP_SOLVER_H

#include <cstddef>
#include <tuple>
#include <vector>
#include <memory>

#include "igraph.h"
#include "solver_graph.h"
#include "solution.h"
#include "common.h"

namespace mtsp {
    class MtspSolver {
    public:

        // imports a complete graph
        void importCompleteGraph(IGraph &graph, std::size_t origin, std::size_t number_of_chains);

        // takes a non-complete graph and converts it to a complete
        void importNonCompleteGraph(IGraph &graph, std::vector<std::size_t> &vertices, std::size_t origin,
                                    std::size_t number_of_chains);

        void solve();

    private:
        unsigned long MaximumSameResults = 10000;
        unsigned long InitialNumber = 1;
        unsigned long MutationNumber = InitialNumber * 4;
        unsigned long CrossoverNumber = InitialNumber * 2;
        unsigned long SelectionGroupSize = (InitialNumber + MutationNumber + CrossoverNumber) / InitialNumber;

        unsigned long objectiveFunction(Solution &solution);
        void generateInitialPopulation();
        void crossover();
        void mutation();
        std::tuple<std::size_t, unsigned long> selection();

        SolverGraph _graph;
        std::size_t _origin = 0;
        std::size_t _number_of_chains = 0;
        std::size_t _number_of_vertices = 0;
        std::vector<unsigned long> _origin_distance;
        std::vector<Solution> _population;
        bool _ready = false;
    };
}


#endif //MTSPSOLVER_MTSP_SOLVER_H

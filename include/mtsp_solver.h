//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_MTSP_SOLVER_H
#define MTSPSOLVER_MTSP_SOLVER_H

#include <tuple>
#include <vector>
#include <memory>

#include "igraph.h"
#include "solution.h"

namespace mtsp {
    class MtspSolver {
    public:

        // a constructor that takes a complete graph
        MtspSolver(IGraph &adjacency_matrix, std::size_t origin, std::size_t number_of_chains);


        void solve();

    private:
        unsigned long MaximumSameResults = 10000;
        unsigned long InitialNumber = 1;
        unsigned long MutationNumer = InitialNumber * 4;
        unsigned long CrossoverNumber = InitialNumber * 2;
        unsigned long SelectionGroupSize = (InitialNumber + MutationNumer + CrossoverNumber) / InitialNumber;

        unsigned long objectiveFunction(Solution &solution);
        void generateInitialPopulation();
        void crossover();
        void mutation();
        std::tuple<std::size_t, unsigned long> selection();

        const std::unique_ptr<const IGraph> _graph;
        const std::size_t _origin;
        const std::size_t _number_of_chains;
        const std::size_t _number_of_vertices;
        std::vector<Solution> _population;
    };
}


#endif //MTSPSOLVER_MTSP_SOLVER_H

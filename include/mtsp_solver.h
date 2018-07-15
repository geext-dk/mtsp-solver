//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_MTSP_SOLVER_H
#define MTSPSOLVER_MTSP_SOLVER_H

#include <tuple>
#include <vector>
#include <memory>

#include "graph.h"
#include "solution.h"

namespace mtsp {
    class MtspSolver {


    public:

        MtspSolver(const Graph &graph, const std::vector<std::size_t> &storage_distance, std::size_t number_of_chains);
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

        const Graph _graph;
        const std::vector<unsigned long> _storage_distance;
        const std::size_t _number_of_chains;
        const std::size_t _number_of_vertices;
        std::vector<Solution> _population;
    };
}


#endif //MTSPSOLVER_MTSP_SOLVER_H

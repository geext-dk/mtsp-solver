//
// Created by geext on 7/8/18.
//

#include "mtsp_solver.h"

namespace mtsp {
    MtspSolver::MtspSolver(const Graph &graph, const std::vector<unsigned long> &storage_distance,
                                 unsigned long number_of_chains)
        : _graph(graph), _storage_distance(storage_distance), _number_of_chains(number_of_chains),
          _number_of_vertices(graph.getNumberOfVertices())
    {
    }

    void MtspSolver::solve() {
        int cycles = 0;

        generateInitialPopulation();

        int i = 0;
        unsigned long best_result = 0;
        unsigned long last_best_result = 0;
        std::size_t best_solution_index = 0;
        while (i < MaximumSameResults) {
            mutation();
            crossover();
            std::tie(best_solution_index, best_result) = selection();

            if (last_best_result != best_result) {
                cycles += 1;
                i = 0;
                last_best_result = best_result;
                std::cout << best_result << '\n';
            } else {
                ++i;
            }
        }
        cycles += i;

        std::cout << "Best result: " << best_result << ". Vertices: ";
        for (std::size_t vertex: _population[best_solution_index].getVertices()) {
            std::cout << vertex << " ";
        }
        std::cout << "\nCycles: " << cycles << std::endl;
    }

    unsigned long MtspSolver::objectiveFunction(mtsp::Solution &solution) {
        unsigned long maximum_distance = 0;
        int chain_start = 0;
        const std::vector<std::size_t> &vertices = solution.getVertices();
        for (std::size_t chain_length : solution.getChainsLengths()) {
            std::size_t previous_vertex = vertices[chain_start];
            unsigned long distance = _storage_distance[previous_vertex];
            for (int position = chain_start + 1; position != chain_start + chain_length; ++position) {
                distance += _graph.getWeight(previous_vertex, vertices[position]);
                previous_vertex = vertices[position];
            }
            distance += _storage_distance[previous_vertex];
            if (distance > maximum_distance) {
                maximum_distance = distance;
            }
            chain_start += chain_length;
        }
        return maximum_distance;
    }

    void MtspSolver::generateInitialPopulation() {
        for (unsigned i = 0; i < InitialNumber; ++i) {
            this->_population.emplace_back(_number_of_vertices, _number_of_chains);
        }
    }

    void MtspSolver::crossover() {
        if (_number_of_vertices < 2 || _population.size() < 2) {
            return;
        }

        for (unsigned number = 0; number < CrossoverNumber; ++number) {
            auto first_chromosome = getRandomInteger<std::size_t>(0, _population.size());
            auto second_chromosome = getRandomInteger<std::size_t>(0, _population.size() - 1);
            if (second_chromosome >= first_chromosome) {
                ++second_chromosome;
            }
            Solution &first = _population[first_chromosome];
            Solution &second = _population[second_chromosome];

            _population.push_back(std::move(Solution::crossover(first, second)));
        }
    }

    void MtspSolver::mutation() {
        int more_than_one_chain = 0;
        if (_number_of_chains >= 2) {
            more_than_one_chain = 1;
        }

        for (unsigned number = 0; number > MutationNumer; ++number) {
            auto chromosome = getRandomInteger<std::size_t>(0, _population.size());
            Solution new_solution(_population[chromosome].clone());

            int chance = more_than_one_chain * getRandomInteger(0, 2);

            if (chance == 0) {
                new_solution.mutateSwap();
            } else {
                new_solution.mutateAdjust();
            }
            _population.push_back(std::move(new_solution));
        }
    }

    std::tuple<std::size_t, unsigned long> MtspSolver::selection() {
        std::vector<std::vector<Solution>> groups(InitialNumber);
        std::vector<unsigned> is_assigned(_population.size(), 0);
        for (std::size_t i = 0; i < _population.size(); ++i) {
            auto position = firstFreeRandom(is_assigned, _population.size() - i);
            groups[i / SelectionGroupSize].push_back(std::move(_population[position]));
            is_assigned[position] = 1;
        }

        std::vector<Solution> new_population;
        std::vector<unsigned long> function_results;

        for (std::vector<Solution> &group : groups) {
            std::size_t minimum_index = 0;
            unsigned long minimum_function_result = objectiveFunction(group[0]);
            for (unsigned i = 0; i < group.size(); ++i) {
                unsigned long function_result = objectiveFunction(group[i]);
                if (minimum_function_result > function_result) {
                    minimum_function_result = function_result;
                    minimum_index = i;
                }
            }
            new_population.push_back(std::move(group[minimum_index]));
            function_results.push_back(minimum_function_result);
        }

        _population.swap(new_population);

        unsigned long minimum = function_results[0];
        std::size_t index = 0;
        for (std::size_t i = 0; i < _population.size(); ++i) {
            if (function_results[i] < minimum) {
                minimum = function_results[i];
                index = i;
            }
        }

        return std::make_tuple(index, function_results[index]);
    }
}


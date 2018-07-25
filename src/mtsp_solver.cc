#include "mtsp_solver.h"
#include <iostream>

namespace mtsp {

void MtspSolver::importAdjacencyList(const AdjacencyList &adjacency_list,
                                     std::vector<std::size_t> &vertices,
                                     std::size_t origin,
                                     std::size_t number_of_chains) {
    _number_of_vertices = vertices.size();
    _number_of_chains = number_of_chains;

    _global_vertices = vertices;

    std::cout << "Converting the graph... " << std::endl;

    _graph.resize(_number_of_vertices);
    for (std::size_t u = 0; u < _number_of_vertices; ++u) {
        std::unordered_map<std::size_t, std::size_t> distance, previous;
        std::tie(distance, previous) = mtsp::dijkstra(adjacency_list,
                                                      _global_vertices.at(u));

        for (std::size_t v = u + 1; v < _number_of_vertices; ++v) {
            _graph.setWeight(u, v, distance.at(_global_vertices.at(v)));
        }

        _graph.setWeight(u, u, std::numeric_limits<unsigned long>::max());
    }
    std::cout << "Creating an origin distance vector" << std::endl;
    std::unordered_map<std::size_t, std::size_t> distance, previous;
    std::tie(distance, previous) = mtsp::dijkstra(adjacency_list, origin);
    _origin_distance.resize(_number_of_vertices);
    for (std::size_t u = 0; u < _number_of_vertices; ++u) {
        _origin_distance.at(u) = distance.at(_global_vertices.at(u));
    }

    std::cout << "Done" << std::endl;
}

void MtspSolver::solve() {
    unsigned cycles = 0;

    generateInitialPopulation();

    unsigned i = 0;
    unsigned long best_result = 0;
    unsigned long last_best_result = 0;
    std::size_t best_solution_index = 0;
    while (i < MaximumSameResults) {
        mutation();
        crossover();
        std::tie(best_solution_index, best_result) = selection();

        if (last_best_result != best_result) {
            cycles += i;
            i = 0;
            last_best_result = best_result;
            std::cout << best_result << '\n';
        } else {
            ++i;
        }
    }
    cycles += i;

    std::cout << "Best result: " << best_result << ". Vertices: ";
    for (std::size_t vertex: _population.at(best_solution_index)
                                                .getVertices()) {
        std::cout << vertex << " ";
    }
    std::cout << "\nCycles: " << cycles << std::endl;
}

const CompleteGraph &MtspSolver::getInternalGraph() const {
    return _graph;
}

unsigned long MtspSolver::objectiveFunction(const Solution &solution) {
    unsigned long maximum_distance = 0;
    std::size_t chain_start = 0;
    const std::vector<std::size_t> &vertices = solution.getVertices();
    for (std::size_t chain_length : solution.getChainsLengths()) {
        std::size_t previous_vertex = vertices.at(chain_start);
        unsigned long distance = _origin_distance.at(previous_vertex);
        for (std::size_t position = chain_start + 1;
                position != chain_start + chain_length;
                ++position) {
            distance += _graph.getWeight(previous_vertex,
                                         vertices.at(position));
            previous_vertex = vertices.at(position);
        }
        distance += _origin_distance.at(previous_vertex);
        if (distance > maximum_distance) {
            maximum_distance = distance;
        }
        chain_start += chain_length;
    }
    return maximum_distance;
}

void MtspSolver::generateInitialPopulation() {
    for (unsigned i = 0; i < InitialNumber; ++i) {
        _population.emplace_back(_number_of_vertices, _number_of_chains);
    }
}

void MtspSolver::crossover() {
    if (_number_of_vertices < 2 || _population.size() < 2) {
        return;
    }

    for (unsigned number = 0; number < CrossoverNumber; ++number) {
        auto first_chromosome = getRandomInteger<std::size_t>(0,
                                                _population.size());
        auto second_chromosome = getRandomInteger<std::size_t>(0,
                                                _population.size() - 1);
        if (second_chromosome >= first_chromosome) {
            ++second_chromosome;
        }
        Solution &first = _population.at(first_chromosome);
        Solution &second = _population.at(second_chromosome);

        _population.push_back(Solution::crossover(first, second));
    }
}

void MtspSolver::mutation() {
    int more_than_one_chain = 0;
    if (_number_of_chains >= 2) {
        more_than_one_chain = 1;
    }

    for (unsigned number = 0; number < MutationNumber; ++number) {
        auto chromosome = getRandomInteger<std::size_t>(0,
                                          _population.size());
        Solution new_solution(_population.at(chromosome).clone());

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
        std::size_t position = firstFreeRandom(is_assigned,
                                              _population.size() - i);
        groups.at(i / SelectionGroupSize).push_back(_population.at(position));
        is_assigned.at(position) = 1;
    }

    std::vector<Solution> new_population;
    std::vector<unsigned long> function_results;

    for (std::vector<Solution> &group : groups) {
        std::size_t minimum_index = 0;
        unsigned long minimum_function_result = objectiveFunction(group.at(0));
        for (unsigned i = 1; i < group.size(); ++i) {
            unsigned long function_result = objectiveFunction(group.at(i));
            if (minimum_function_result > function_result) {
                minimum_function_result = function_result;
                minimum_index = i;
            }
        }
        new_population.push_back(group.at(minimum_index));
        function_results.push_back(minimum_function_result);
    }

    _population.swap(new_population);

    unsigned long minimum = function_results.at(0);
    std::size_t index = 0;
    for (std::size_t i = 0; i < _population.size(); ++i) {
        if (function_results.at(i) < minimum) {
            minimum = function_results.at(i);
            index = i;
        }
    }

    return std::make_tuple(index, function_results.at(index));
}

}


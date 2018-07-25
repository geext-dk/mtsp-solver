#include "solution.h"

#include <iostream>
#include "util.h"

namespace mtsp {

Solution::Solution() : _number_of_vertices(0), _number_of_chains(0) { }

Solution::Solution(const std::vector<std::size_t> &vertices,
                   const std::vector<std::size_t> &chains_lengths)
        : _vertices(vertices),
          _chains_lengths(chains_lengths),
          _number_of_vertices(vertices.size()),
          _number_of_chains(chains_lengths.size()) { }

Solution::Solution(std::vector<std::size_t> &&vertices,
                   const std::vector<std::size_t> &chains_lengths)
        : _vertices(std::move(vertices)),
          _chains_lengths(chains_lengths),
          _number_of_vertices(vertices.size()),
          _number_of_chains(chains_lengths.size()) { }

Solution::Solution(std::size_t number_of_vertices,
                   std::size_t number_of_chains)
        : _number_of_vertices(number_of_vertices),
          _number_of_chains(number_of_chains) {
    regenerateChainsLengths();
    regenerateVertices();
}

const std::vector<std::size_t>& Solution::getVertices() const {
    return _vertices;
}

const std::vector<std::size_t>& Solution::getChainsLengths() const {
    return _chains_lengths;
}

Solution Solution::crossover(const mtsp::Solution &first_parent,
                             const mtsp::Solution &second_parent) {
    const std::vector<std::size_t> &second_vertices = second_parent
                                                        .getVertices();

    std::vector<std::size_t> child_vertices(first_parent.getVertices());
    std::size_t number_of_vertices = child_vertices.size();
    std::vector<int> is_position_assigned(number_of_vertices, 0);
    std::vector<int> is_vertex_assigned(number_of_vertices, 0);

    auto length = getRandomInteger<std::size_t>(1, number_of_vertices - 1);
    auto start_with = getRandomInteger<std::size_t>(1,
                                            number_of_vertices - length);

    for (std::size_t i = start_with; i < length + start_with; ++i) {
        is_position_assigned.at(i) = 1;
        is_vertex_assigned.at(child_vertices.at(i)) = 1;
    }

    for (std::size_t i = 0; i < number_of_vertices; ++i) {
        if (is_position_assigned.at(i)) {
            continue;
        }

        for (std::size_t second_vertex : second_vertices) {
            if (!is_vertex_assigned.at(second_vertex)) {
                child_vertices.at(i) = second_vertex;
                is_position_assigned.at(i) = 1;
                is_vertex_assigned.at(second_vertex) = 1;
                break;
            }
        }
    }

    return Solution(std::move(child_vertices), first_parent.getChainsLengths());
}

Solution Solution::clone() const {
    return Solution(_vertices, _chains_lengths);
}

void Solution::regenerateChainsLengths() {
    _chains_lengths.resize(_number_of_chains);

    std::vector<unsigned> is_assigned(_number_of_chains, 0);

    unsigned number_of_assigned_vertices = 0;
    for (std::size_t i = 0; i < _number_of_chains - 1; ++i) {
        // upper bound to generate a number of vertices assigned to the chain
        // constraint: at least 2 vertices per chain
        // so I reserve 2 vertices for every chain left to be assigned excluding the current processed chain
        // 2 * (_number_of_chains - (i + 1)) <- this
        // then I subtract the number of already assigned vertices
        // (we can't assign them as they are already assigned)
        std::size_t upper_bound = _number_of_vertices;
        upper_bound -= 2 * (_number_of_chains - (i + 1));
        upper_bound -= number_of_assigned_vertices;
        std::size_t random_number_of_vertices = getRandomInteger<std::size_t>(2,
                                                               upper_bound + 1);
        std::size_t random_chain = firstFreeRandom(is_assigned,
                                                   _number_of_chains - i);
        _chains_lengths.at(random_chain) = random_number_of_vertices;
        is_assigned.at(random_chain) = 1;
        number_of_assigned_vertices += random_number_of_vertices;
    }

    std::size_t last_chain;
    for (last_chain = 0; last_chain < _number_of_chains; ++last_chain) {
        if (is_assigned.at(last_chain) == 0) {
            break;
        }
    }

    _chains_lengths.at(last_chain) = _number_of_vertices - number_of_assigned_vertices;
}

void Solution::regenerateVertices() {
    _vertices.resize(_number_of_vertices);
    std::vector<unsigned> assigned(_number_of_vertices, 0);
    for (std::size_t i = 0; i < _number_of_vertices; ++i) {
        std::size_t random_vertex = firstFreeRandom(assigned,
                                                    _number_of_vertices - i);
        assigned.at(random_vertex) = 1;
        _vertices.at(i) = random_vertex;
    }
}

void Solution::mutateSwap() {
    auto first_vertex = getRandomInteger<std::size_t>(0, _number_of_vertices);
    auto second_vertex = getRandomInteger<std::size_t>(0,
                                                 _number_of_vertices - 1);
    if (second_vertex >= first_vertex) {
        ++second_vertex;
    }
    std::swap(_vertices.at(first_vertex), _vertices.at(second_vertex));
}

void Solution::mutateAdjust() {
    std::size_t first_chain = getRandomInteger<std::size_t>(0,
                                                        _number_of_chains);
    std::size_t second_chain = getRandomInteger<std::size_t>(0,
                                                        _number_of_chains - 1);
    if (second_chain >= first_chain) {
        ++second_chain;
    }

    if (_chains_lengths.at(first_chain) <= 2) {
        return;
    }

    std::size_t adjustment = getRandomInteger<std::size_t>(1,
                                        _chains_lengths.at(first_chain) - 1);

    _chains_lengths.at(first_chain) -= adjustment;
    _chains_lengths.at(second_chain) += adjustment;
}

}

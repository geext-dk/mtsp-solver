//
// Created by geext on 7/8/18.
//

#include "solution.h"

namespace mtsp {

    Solution::Solution(const std::vector<unsigned long> &vertices, const std::vector<unsigned long> &chains_lengths)
            : _number_of_chains(chains_lengths.size()), _number_of_vertices(vertices.size()),
              _vertices(vertices), _chains_lengths(chains_lengths) {
    }

    Solution::Solution(unsigned long number_of_vertices, unsigned long number_of_chains)
            : _number_of_vertices(number_of_vertices), _number_of_chains(number_of_chains) {
        regenerateChainsLengths();
        regenerateVertices();
    }

    const std::vector<unsigned long>& Solution::getVertices() const {
        return _vertices;
    }

    const std::vector<unsigned long>& Solution::getChainsLengths() const {
        return _chains_lengths;
    }

    Solution Solution::clone() const {
        return Solution(_vertices, _chains_lengths);
    }

    void Solution::regenerateChainsLengths() {
        _chains_lengths.resize(_number_of_chains);

        std::vector<int> assigned(_number_of_chains, 0);

        int number_of_assigned_vertices = 0;
        for (unsigned long i = 0; i < _number_of_chains - 1; ++i) {
            // upper bound to generate a number of vertices assigned to the chain
            // constraint: at least 2 vertices per chain
            // so I reserve 2 vertices for every chain left to be assigned excluding the current processed chain
            // 2 * (_number_of_chains - (i + 1)) <- this
            // then I subtract the number of already assigned vertices
            // (we can't assign them as they are already assigned)
            unsigned long upper_bound = _number_of_vertices;
            upper_bound -= 2 * (_number_of_chains - (i + 1));
            upper_bound -= number_of_assigned_vertices;
            std::uniform_int_distribution<unsigned long> generator(2, upper_bound);
            unsigned long random_number_of_vertices = generator(getRandomEngine());
            unsigned long random_chain = firstFreeRandom(assigned, _number_of_chains - i);
            _chains_lengths[random_chain] = random_number_of_vertices;
            assigned[random_chain] = 1;
            number_of_assigned_vertices += random_number_of_vertices;
        }

        unsigned long last_chain;
        for (last_chain = 0; last_chain < _number_of_chains; ++last_chain) {
            if (assigned[last_chain] == 0) {
                break;
            }
        }

        _chains_lengths[last_chain] = _number_of_vertices - number_of_assigned_vertices;
    }

    void Solution::regenerateVertices() {
        _vertices.resize(_number_of_vertices);
        std::vector<int> assigned(_number_of_vertices, 0);
        for (unsigned long i = 0; i < _number_of_vertices; ++i) {
            unsigned long random_vertex = firstFreeRandom(assigned, _number_of_vertices - i);
            assigned[random_vertex] = 1;
            _vertices[i] = random_vertex;
        }
    }
}

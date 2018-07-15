//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_SOLUTION_H
#define MTSPSOLVER_SOLUTION_H

#include <vector>

#include "common.h"

namespace mtsp {
    /// A class that represents a single solution to the problem. It consists of two vectors. The first vector,
    /// chains_lengths, which size is number_of_chains, contains lengths of every chain so chains_lengths[0] is a
    /// length of a first chain and so on. The second vector represents vertices in that order so the first chain
    /// consists of first chains_lengths[0] vertices, second chain consists of next chains_lengths[1] vertices and so
    /// on.
    class Solution {

    public:
        Solution();

        /// The constructor copies the vertices given and creates a solution based on them
        /// @param vertices        A vertices vector
        /// @param chains_lengths  A chains lengths vector
        Solution(const std::vector<std::size_t> &vertices, const std::vector<std::size_t> &chains_lengths);

        /// The constructor generates a randomized solution based on parameters
        /// @param number_of_chains    The exact number of chains between which the vertices will be distributed
        /// @param number_of_vertices  Number of vertices in a graph
        Solution(std::size_t number_of_vertices, std::size_t number_of_chains);

        static Solution crossover(const Solution &first_parent, const Solution &second_parent);

        /// Access the solution's vertices
        /// @returns Solution's vertices vector
        const std::vector<std::size_t>& getVertices() const;

        /// Access solution's chains_lengths vector
        /// @returns Solution's chains_length vector
        const std::vector<std::size_t>& getChainsLengths() const;

        /// Copy the solution
        /// @returns A copy of the solution
        Solution clone() const;

        void mutateSwap();

        void mutateAdjust();

    private:
        std::vector<std::size_t> _vertices;
        std::vector<std::size_t> _chains_lengths;
        const std::size_t _number_of_vertices;
        const std::size_t _number_of_chains;

        // functions to regenerate the corresponding arrays
        void regenerateChainsLengths();
        void regenerateVertices();
    };
}


#endif //MTSPSOLVER_SOLUTION_H

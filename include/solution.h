//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_SOLUTION_H
#define MTSPSOLVER_SOLUTION_H

#include <vector>
#include <random>

#include "common.h"

namespace mtsp {
    class Solution {

    public:
        Solution(const std::vector<unsigned long> &vertices, const std::vector<unsigned long> &chains_lengths);
        Solution(unsigned long number_of_vertices, unsigned long number_of_chains);
        const std::vector<unsigned long>& getVertices() const;
        const std::vector<unsigned long>& getChainsLengths() const;
        Solution clone() const;

    private:
        std::vector<unsigned long> _vertices;
        std::vector<unsigned long> _chains_lengths;
        const unsigned long _number_of_vertices;
        const unsigned long _number_of_chains;

        // functions to regenerate the corresponding arrays
        void regenerateChainsLengths();
        void regenerateVertices();
    };
}


#endif //MTSPSOLVER_SOLUTION_H

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
        Solution(std::vector<unsigned long> &shops_array, std::vector<unsigned long> &paths_lengths);
        Solution(unsigned long number_of_shops, unsigned long number_of_cars);
        const std::vector<unsigned long>& getShopsArray() const;
        const std::vector<unsigned long>& getPathsLengths() const;
        Solution&& copy() const;

    private:
        std::vector<unsigned long> _shops_array;
        std::vector<unsigned long> _paths_lengths;
        const unsigned long _number_of_shops;
        const unsigned long _number_of_cars;

        void regeneratePathsLengths();
        void regenerateShopsArray();


    };
}


#endif //MTSPSOLVER_SOLUTION_H

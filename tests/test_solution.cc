//
// Created by geext on 7/11/18.
//

#include <gtest/gtest.h>
#include "solution.h"

// The other constructor is so trivial... doesn't need to be tested I suppose
TEST(TestSolution, RandomConstructor) {
    unsigned long number_of_vertices = 100;
    unsigned long number_of_chains = 7;
    unsigned long sum = 0;

    // a subject
    mtsp::Solution solution(number_of_vertices, number_of_chains);

    // check the generated vectors sizes
    ASSERT_EQ(number_of_chains, solution.getChainsLengths().size());
    ASSERT_EQ(number_of_vertices, solution.getVertices().size());

    // check the generated chains_lengths vector
    for (unsigned long chain_length : solution.getChainsLengths()) {
        sum += chain_length;
    }
    ASSERT_EQ(number_of_vertices, sum);

    // check the generated vertices vector
    std::vector<int> binary_vector(number_of_vertices, 0);
    std::cout << std::endl;
    for (unsigned long vertex : solution.getVertices()) {
        ASSERT_EQ(0, binary_vector[vertex]);
        binary_vector[vertex] = 1;
    }
}

#include <gtest/gtest.h>
#include <iostream>
#include "solution.h"
using namespace mtsp;

// The other constructor is so trivial... doesn't need to be tested I suppose
TEST(TestSolution, RandomConstructor) {
    unsigned long number_of_vertices = 50;
    unsigned long number_of_chains = 7;
    unsigned long sum = 0;

    // a subject
    Solution solution(number_of_vertices, number_of_chains);

    // check the generated vectors sizes
    ASSERT_EQ(number_of_chains, solution.getChainsLengths().size());
    ASSERT_EQ(number_of_vertices, solution.getVertices().size());

    // check the generated vertices vector
    std::vector<int> binary_vector(number_of_vertices, 0);
    std::cout << std::endl;
    for (unsigned long vertex : solution.getVertices()) {
        std::cout << vertex << " ";
        ASSERT_EQ(0, binary_vector[vertex]);
        binary_vector[vertex] = 1;
    }

    std::cout << "| ";

    // check the generated chains_lengths vector
    for (unsigned long chain_length : solution.getChainsLengths()) {
        std::cout << chain_length << " ";
        sum += chain_length;
    }
    ASSERT_EQ(number_of_vertices, sum);
}


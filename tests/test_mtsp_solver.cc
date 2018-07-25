#include <gtest/gtest.h>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <iostream>
#include "mtsp_solver.h"
using namespace mtsp;

std::tuple<std::size_t, std::size_t, std::vector<unsigned long>, AdjacencyList>
readInput(const std::string& file_path)
{
    std::ifstream input_file(file_path);

    if (!input_file.good()) {
        throw std::invalid_argument("The argument must be an existing path");
    }

    // get first line - number of chains and an origin vertex
    std::string line;
    std::getline (input_file, line);
    std::istringstream stream;
    stream.str(line);

    unsigned long number_of_chains = 0;
    std::size_t origin_vertex = 0;
    stream >> number_of_chains;
    std::cout << "Number of chains: " << number_of_chains << std::endl;
    stream >> origin_vertex;
    std::cout << "Origin: " << origin_vertex << std::endl;
    stream.clear();

    // get second line - an array of vertices for mTSP
    std::getline(input_file, line);
    stream.str(line);

    std::size_t vertex;
    std::vector<std::size_t> vertices;
    if (!stream.good()) {
        throw std::invalid_argument("what?");
    }
    while (stream >> vertex) {
        vertices.push_back(vertex);
    }
    stream.clear();

    std::cout << "Number of vertices: " << vertices.size() << std::endl;

    // all other lines contain edges
    AdjacencyList adjacency_list;
    while (std::getline(input_file, line)) {
        stream.str(line);
        std::size_t first_vertex;
        std::size_t second_vertex;
        unsigned long weight;
        stream >> first_vertex >> second_vertex >> weight;
        adjacency_list.addEdge(first_vertex, second_vertex, weight);
        stream.clear();
    }
    return std::make_tuple(number_of_chains, origin_vertex,
                           std::move(vertices), std::move(adjacency_list));
}

TEST (TestMtspSolver, Constructor) {
    MtspSolver solver;
    AdjacencyList list({
        {1, {EndVertex(2, 5), EndVertex(3, 7)}},
        {2, {EndVertex(1, 5), EndVertex(5, 9)}},
        {3, {EndVertex(1, 7), EndVertex(5, 1)}},
        {5, {EndVertex(2, 9), EndVertex(3, 1)}}
    });
    std::vector<std::size_t> vertices {1, 2, 3, 5};
    solver.importAdjacencyList(list, vertices, 1, 1);
}

TEST (TestMtspSolver, SolveFromInputFile) {
    AdjacencyList list;
    std::vector<std::size_t> vertices;
    std::size_t number_of_chains, origin_vertex;
    std::tie(number_of_chains, origin_vertex, vertices, list)
        = readInput("input.txt");
    MtspSolver solver;
    solver.importAdjacencyList(list, vertices, origin_vertex, number_of_chains);
    solver.solve();
}

//
// Created by geext on 7/15/18.
//

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "end_vertex.h"
#include "common.h"
#include "mtsp_solver.h"
#include "adjacency_list.h"
#include "igraph.h"

std::tuple<std::size_t, std::size_t, std::vector<std::size_t>, AdjacencyList> readInput(const std::string& file_path)
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
    return std::make_tuple(number_of_chains, origin_vertex, std::move(vertices), std::move(adjacency_list));
}

std::tuple<mtsp::Graph, std::vector<std::size_t>>
convertGraph(const AdjacencyList &adjacency_list,
             const std::vector<std::size_t> &vertices,
             const std::size_t origin_vertex)
{
    std::size_t number_of_vertices = vertices.size();
    std::cout << "Converting the graph... " << std::endl;
    mtsp::Graph graph(number_of_vertices);

    for (std::size_t vertex_num = 0; vertex_num < number_of_vertices; ++vertex_num) {
        std::unordered_map<std::size_t, std::size_t> distance, previous;
        std::tie(distance, previous) = mtsp::dijkstra(adjacency_list, vertices[vertex_num]);

        for (std::size_t adjacent_vertex_num = 0; adjacent_vertex_num < number_of_vertices; ++adjacent_vertex_num) {
            graph.setWeight(vertex_num, adjacent_vertex_num, distance[vertices[adjacent_vertex_num]]);
        }

        graph.setWeight(vertex_num, vertex_num, std::numeric_limits<unsigned long>::max());
    }
    std::cout << "Done matrix conversion" << std::endl;

    std::vector<unsigned long> origin_distance;
    std::unordered_map<std::size_t, std::size_t> distance, previous;
    std::tie(distance, previous) = mtsp::dijkstra(adjacency_list, origin_vertex);
    for (std::size_t vertex_num = 0; vertex_num < number_of_vertices; ++vertex_num) {
        origin_distance.push_back(distance[vertices[vertex_num]]);
    }

    std::cout << "Done" << std::endl;
    return std::make_tuple(std::move(graph), std::move(origin_distance));
}

int main() {
    // read input
    std::size_t number_of_chains, origin_vertex;
    std::vector<std::size_t> vertices;
    AdjacencyList adjacency_list;
    std::tie(number_of_chains, origin_vertex, vertices, adjacency_list) = readInput("/home/geext/CLionProjects/MtspSolver/input.txt");

    // convert to a complete graph
    mtsp::Graph graph;
    std::vector<unsigned long> origin_distance;
    std::tie(graph, origin_distance) = convertGraph(adjacency_list, vertices, origin_vertex);

    // create a solver
    mtsp::MtspSolver solver(graph, origin_distance, number_of_chains);

    // solve the problem
    solver.solve();
}
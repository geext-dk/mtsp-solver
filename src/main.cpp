//
// Created by geext on 7/15/18.
//

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "end_vertex.h"
#include "common.h"
#include "mtsp_solver.h"

std::tuple<std::size_t, std::size_t, std::vector<std::size_t>,
        std::unordered_map<std::size_t, std::vector<mtsp::EndVertex>>>
readInput(const std::filesystem::path& file_path) {
    unsigned long number_of_chains = 0;
    std::size_t origin_vector = 0;
    std::unordered_map<std::size_t, std::vector<mtsp::EndVertex>> adjacency_list;
    std::vector<std::size_t> vertices;

    if (!std::filesystem::exists(file_path)) {
        throw std::invalid_argument("The argument must be an existing path");
    } else if (!std::filesystem::is_regular_file(file_path)) {
        throw std::invalid_argument("The argument must be a regular file");
    }

    std::ifstream input_file(file_path);
    std::string line;
    std::getline(input_file, line);

    std::istringstream stream;
    stream.str(line);

    stream >> number_of_chains;
    std::cout << "Number of chains: " << number_of_chains << std::endl;
    stream >> origin_vector;
    std::cout << "Origin: " << origin_vector << std::endl;

    std::getline(input_file, line);
    stream.str(line);

    std::size_t vertex;
    while (stream >> vertex) {
        vertices.push_back(vertex);
    }

    std::cout << "Number of vertices: " << vertices.size() << std::endl;

    // read vertices
    while (std::getline(input_file, line)) {
        stream.str(line);
        std::size_t first_vertex;
        std::size_t second_vertex;
        unsigned long weight;
        stream >> first_vertex >> second_vertex >> weight;
        mtsp::EndVertex first_end(first_vertex, weight);
        mtsp::EndVertex second_end(second_vertex, weight);
        adjacency_list[first_vertex].push_back(second_end);
        adjacency_list[second_vertex].push_back(first_end);
    }
    return std::make_tuple(number_of_chains, origin_vector, std::move(vertices), std::move(adjacency_list));
}

std::tuple<std::vector<std::vector<unsigned long>>, std::vector<std::size_t>>
convertGraph(const std::unordered_map<std::size_t, std::vector<mtsp::EndVertex>> &adjacency_list,
                  const std::vector<std::size_t> &vertices,
                  const std::size_t origin_vector)
{
    std::size_t number_of_vertices = vertices.size();
    std::cout << "Converting the graph... " << std::endl;
    std::vector<std::vector<std::size_t>> matrix(number_of_vertices, std::vector<std::size_t>(number_of_vertices, 0));

    for (std::size_t vertex = 0; vertex < number_of_vertices; ++vertex) {
        auto [distance, previous] = mtsp::dijkstra(adjacency_list, vertices[vertex]);

        for (std::size_t adjacent_vertex = 0; adjacent_vertex < number_of_vertices; ++adjacent_vertex) {
            matrix[vertex][adjacent_vertex] = distance[vertices[adjacent_vertex]];
            matrix[adjacent_vertex][vertex] = distance[vertices[adjacent_vertex]];
        }

        matrix[vertex][vertex] = std::numeric_limits<std::size_t>::max();
    }

    std::vector<unsigned long> origin_distance;
    auto [distance, previous] = mtsp::dijkstra(adjacency_list, origin_vector);
    for (std::size_t vertex = 0; vertex < number_of_vertices; ++vertex) {
        origin_distance[vertex] = distance[vertices[vertex]];
    }

    std::cout << "Done" << std::endl;
    return std::make_tuple(std::move(matrix), std::move(origin_distance));
}

int main() {
    auto [number_of_chains, origin_vector, vertices, adjacency_list]
        = readInput(std::filesystem::path("/home/geext/CLionProjects/MtspSolver/input.txt"));
    auto [adjacency_matrix, origin_distance] = convertGraph(adjacency_list, vertices, origin_vector);
    mtsp::MtspSolver solver(mtsp::Graph(adjacency_matrix), origin_distance, number_of_chains);
    solver.solve();
}
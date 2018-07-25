cmake_minimum_required(VERSION 3.10)
project(MtspSolver)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(MtspSolver 
            src/adjacency_list.cc
            src/util.cc
            src/end_vertex.cc
            src/mtsp_solver.cc
            src/destination.cc
            src/complete_graph.cc
            src/solution.cc
            src/priority_queue.cc
            include/adjacency_list.h
            include/util.h
            include/end_vertex.h
            include/mtsp_solver.h
            include/destination.h
            include/complete_graph.h
            include/solution.h
            include/priority_queue.h
)

# unit tests
add_subdirectory(tests)

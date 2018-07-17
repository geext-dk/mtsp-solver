cmake_minimum_required(VERSION 3.10)
project(MtspSolver)

set(CMAKE_CXX_STANDARD 11)

set(IDIR include)
set(SDIR src)
include_directories(${IDIR})

set(HDRS "")
set(SRCS "")

set(_FILES common end_vertex mtsp_solver solution destination adjacency_list)
foreach(name ${_FILES})
  list(APPEND HDRS "${IDIR}/${name}.h")
  list(APPEND SRCS "${SDIR}/${name}.cc")
endforeach(name)

add_library(MtspSolver ${HDRS} include/igraph.h ${SRCS})

project(MtspSolverMain)
add_executable(MtspSolverMain src/main.cpp ${HDRS})
target_link_libraries(MtspSolverMain MtspSolver stdc++fs)

# unit tests
add_subdirectory(tests)

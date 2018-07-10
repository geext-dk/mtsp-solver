cmake_minimum_required(VERSION 3.10)
project(MtspSolverTests)

set(CMAKE_CXX_STANDARD 11)

add_executable(MtspSolverTests common.cc)

target_link_libraries(MtspSolverTests MtspSolver gtest gtest_main)

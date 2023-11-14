cmake_minimum_required(VERSION 3.22)

project(nBody VERSION 1.0 LANGUAGES CXX)

# Ensure cmake can find conan libraries
set(Eigen3_DIR ${CMAKE_BINARY_DIR})
set(Catch2_DIR ${CMAKE_BINARY_DIR})

# Make executables appear in build, not build/src
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Adding optimization flags

# without optimisation
# add_compile_options(-O0) 

# with optimisation
add_compile_options(-O2) 

# Build application
add_subdirectory(app)

# Build library
add_subdirectory(src)

# Build tests
add_subdirectory(test)
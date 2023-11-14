#/*============================================================================
#
#  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.
#
#  Copyright (c) University College London (UCL). All rights reserved.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
#  See LICENSE.txt in the top level directory for details.
#
#============================================================================*/

cmake_minimum_required(VERSION 3.10)
project(PHAS0100ASSIGNMENT1 VERSION 0.0.0)


# Use files in CMake dir
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

# Enable testing; disable lib building
option(BUILD_TESTING "Build Unit tests." ON)
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)

# Add test CMake config
include(golSetupTesting)

# Set build type if not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Valid options are Release or Debug" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug")
endif()

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
  message(FATAL_ERROR "Build type \"${CMAKE_BUILD_TYPE}\" is not supported.")
endif()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS 0)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# Copy useful files to build dir
configure_file(${CMAKE_SOURCE_DIR}/LICENSE.txt ${CMAKE_BINARY_DIR}/LICENSE.txt @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/LICENSE.txt DESTINATION . COMPONENT CONFIG)
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.md @ONLY)

# Help IDE find folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


######################################################################
# A few shortcuts for lists of libraries.
######################################################################
set(PHAS0100ASSIGNMENT1_LIBRARIES PHAS0100ASSIGNMENT1)
set(ALL_LIBRARIES ${PHAS0100ASSIGNMENT1_LIBRARIES})


######################################################################
# Set up a few paths.
######################################################################
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(PHAS0100ASSIGNMENT1_INSTALL_LIB_DIR lib)
set(PHAS0100ASSIGNMENT1_INSTALL_INC_DIR include)
set(PHAS0100ASSIGNMENT1_INSTALL_BIN_DIR bin)

foreach(type LIBRARY RUNTIME ARCHIVE)
  set(output_dir ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_${type}_OUTPUT_DIRECTORY ${output_dir} CACHE INTERNAL "Single output directory for building all libraries.")
  mark_as_advanced(CMAKE_${type}_OUTPUT_DIRECTORY)
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${CMAKE_BINARY_DIR})


######################################################################
# Add our main code folders. This is where all our functionality is.
######################################################################
add_subdirectory(src)
if(BUILD_TESTING)
  set(TEMP_DIR ${CMAKE_BINARY_DIR}/test/temp)
  include_directories(${CMAKE_SOURCE_DIR}/test/)
  add_subdirectory(test)
endif()

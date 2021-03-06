# ----------------------------------------------------------------------------
#  Root CMake file for Nomad
# ----------------------------------------------------------------------------

# cmake version : the latest one
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# name of the project
project(Nomad
    VERSION 4.0.0
    LANGUAGES C CXX)

# use standard compilers parameters for c++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "FATAL: In-source builds are not allowed.
    You should create a separate directory for build files.
    ")
endif()

# Check compiler version
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
        message(FATAL_ERROR "GCC version < 8 has not been tested for Nomad!")
    endif()
elseif (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") 
    OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    # require at least clang 5
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
        message(FATAL_ERROR "Clang version has not been tested for Nomad!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()

# Options

# build Nomad with openMP (only work with gcc compiler)
option(NOMAD_WITH_OPENMP "Enable OpenMP flags." ON)

# build Nomad examples
option(NOMAD_WITH_EXAMPLES "Build Nomad examples." ON)

# choose to build with unit tests
# option(BUILD_TESTS "Build unit tests programs" OFF)

# choose to build with benchmarks
# option(BUILD_BENCHMARKS "Build benchmark tests" OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# special flag required
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add sgtelib
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext)

# Add nomad directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

if (NOMAD_WITH_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

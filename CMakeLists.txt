cmake_minimum_required(VERSION 3.19)
project(face_touching_detector)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)

option(BUILD_TESTS "Build Unit Tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(deps/spdlog)
add_subdirectory(src)


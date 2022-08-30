cmake_minimum_required(VERSION 3.19)
project(face_touching_detector)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)


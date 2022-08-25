cmake_minimum_required(VERSION 3.19)
project(face_touching_detector_raspi)

set(CMAKE_CXX_STANDARD 14)
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${DEPS_DIR}/libtorch)

find_package(OpenCV 4 REQUIRED)
#find_package(Torch REQUIRED)
find_package(spdlog REQUIRED)

#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS})

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries (${PROJECT_NAME}
        ${OpenCV_LIBS}
#        ${TORCH_LIBRARIES}
        )
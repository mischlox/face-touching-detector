set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
set(CMAKE_PREFIX_PATH ${DEPS_DIR}/libtorch)
find_package(OpenCV 4 REQUIRED)
find_package(Torch REQUIRED)
find_package(spdlog REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(DETECTOR_LIB ft-detector)
add_library(${DETECTOR_LIB} detector.cpp yolov5.cpp)
target_include_directories(${DETECTOR_LIB} PUBLIC include)

target_link_libraries(${DETECTOR_LIB}
        ${OpenCV_LIBS}
        ${TORCH_LIBRARIES}
        )
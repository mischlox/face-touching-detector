list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/deps/libtorch")
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(DETECTOR_LIB ft-detector)
add_library(${DETECTOR_LIB} SHARED detector.cpp yolov5.cpp bbox_intersect_listener.cpp)
target_include_directories(${DETECTOR_LIB} PUBLIC include)

target_link_libraries(${DETECTOR_LIB} PUBLIC
        ${OpenCV_LIBS}
        ${TORCH_LIBRARIES}
        spdlog::spdlog_header_only
        )
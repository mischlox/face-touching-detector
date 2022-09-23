add_subdirectory(ft-detector)
add_subdirectory(ft-detector-app)

find_package(Torch REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME}
        ft-detector
        ${Boost_LIBRARIES}
        spdlog::spdlog_header_only)
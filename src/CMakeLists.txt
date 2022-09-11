add_subdirectory(ft-detector)
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME}
        ft-detector
        ${Boost_LIBRARIES})
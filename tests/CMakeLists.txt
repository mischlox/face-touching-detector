find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SUITE test_ftdetector)
add_executable(${TEST_SUITE} yolov5_test.cpp)
target_link_libraries(${TEST_SUITE} PRIVATE
        GTest::GTest
        GTest::Main
        ft-detector
        )

add_test(face-touching-tests ${TEST_SUITE})
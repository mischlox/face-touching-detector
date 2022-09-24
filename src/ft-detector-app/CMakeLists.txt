set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(Torch REQUIRED)

add_executable(ft-detector-app
        main_gui.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        detector_qt.cpp
        detector_qt.h
        ${CMAKE_SOURCE_DIR}/res/resources.qrc
        )

target_link_libraries(ft-detector-app PRIVATE ft-detector Qt5::Widgets Qt5::Multimedia)

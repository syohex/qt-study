cmake_minimum_required(VERSION 3.1.0)

project(Sample02 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
    set(QT5_DIR "$ENV{QTDIR}/lib/cmake/Qt5")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(Sample02
    main.cpp
)

target_link_libraries(Sample02 Qt5::Widgets Qt5::Network)
